#include <mntEdgesLocator.h>
#include <netcdf.h>
#include <iostream>
#include <algorithm>
#include <cmath>

#define LON_INDEX 0
#define LAT_INDEX 1
#define ELV_INDEX 2

EdgesLocator::EdgesLocator() {
}

/**
 * Destructor
 */
EdgesLocator::~EdgesLocator() {
}

void
EdgesLocator::build(const std::vector<double>& edge2Points, int numEdgesPerBucket) {

    // required to test if a line has any chance of intersecting a cell
    const double halo = 0.12;

    size_t numEdges = edge2Points.size() / (NUM_SPACE_DIMS * 2);

    // number of cells in x and y directions
    this->nBuckets = std::max(1, (int)(std::sqrt((double)numEdges/(2.0 * (double) numEdgesPerBucket))));

    // create empty entries 
    for (size_t i = 0; i < this->nBuckets; ++i) {
        for (size_t j = 0; j < this->nBuckets; ++j) {
            // flat index
            size_t k = j + i * this->nBuckets;
            std::pair<size_t, std::vector<size_t> > kv(k, std::vector<size_t>());
            this->buckets.insert(kv);
        }
    }

    for (size_t ie = 0; ie < numEdges; ++ie) {

        // get the start/end point of the edge
        const double* pBeg = &edge2Points[0 + (0 + ie*2)*NUM_SPACE_DIMS];
        const double* pEnd = &edge2Points[0 + (1 + ie*2)*NUM_SPACE_DIMS];

        // compute the start/end points of line in bucket space
        Vector<double> bBeg = this->getBucketSpaceLoc(pBeg);
        Vector<double> bEnd = this->getBucketSpaceLoc(pEnd);
        Vector<double> bU = bEnd - bBeg;
        double bUNormSquare = dot(bU, bU);

        // compute the bucket index location
        Vector<int> iBeg = this->getBucketCellLoc(pBeg);
        Vector<int> iEnd = this->getBucketCellLoc(pEnd);

        // select the region of interest
        Vector<int> imin = min(iBeg, iEnd);
        Vector<int> imax = max(iBeg, iEnd);

        // iterate over all the cells in the region of interest
        for (int i = imin[0]; i <= imax[0]; ++i) {
            for (int j = imin[1]; j <= imax[1]; ++j) {

                // average the vertex positions
                Vector<double> bQuadCentre(NUM_PARAM_DIMS, 0.0);
                bQuadCentre[0] = i + 0.5; bQuadCentre[1] = j + 0.5;

                // check if line intersects with this bucket. Compute the line parameter that
                // minimizes the distance of the cell's vertices to the line. Then check if this
                // point is inside the quad to within some tolerance (halo). Now it can happen 
                // that the clostest point is outside but the line still interects the bucket, e.g.
                // by cutting the corner.  
                double lam = dot(bQuadCentre - bBeg, bU)/bUNormSquare;
                lam = std::min(1.0, lam);
                lam = std::max(0.0, lam);

                // point on the line that is closest to the quad
                Vector<double> bClosestPtOnLine = bBeg + lam*bU;

                // is the point inside the quad to within some tolerance?
                if (bClosestPtOnLine[0] >= i - halo && bClosestPtOnLine[0] <= i + 1 + halo &&
                    bClosestPtOnLine[1] >= j - halo && bClosestPtOnLine[1] <= j + 1 + halo) {

                    // flat index
                    size_t k = j + i * this->nBuckets;

                    std::map<size_t, std::vector<size_t> >::iterator it = this->buckets.find(k);

                    it->second.push_back(ie);
                    
                } // test if closest point is inside bucket
            } // j bucket iteration
        } // i bucket iteration
    } // edge iteration
}

std::set<vtkIdType> 
EdgesLocator::getEdgesAlongLine(const double pBeg[], const double pEnd[]) const {

    Vector<int> iBeg = this->getBucketCellLoc(pBeg);
    Vector<int> iEnd = this->getBucketCellLoc(pEnd);
    Vector<int> imin = min(iBeg, iEnd);
    Vector<int> imax = max(iBeg, iEnd);

    // collect edge ids
    std::set<vtkIdType> edgeIds;

    for (int i = imin[0]; i <= imax[0]; ++i) {
        for (int j = imin[1]; j <= imax[1]; ++j) {

            // flat index
            size_t k = j + i * this->nBuckets;

            std::map<size_t, std::vector<size_t> >::const_iterator it = this->buckets.find(k);
            for (auto ie : it->second) {
                // add all the edges that belong to that cell
                edgeIds.insert(ie);
            }
        }
    }

    return edgeIds;
}

