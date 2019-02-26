#include <vector>
#include <set>
#include <algorithm>
#include "MvVector.h"
#include <map>
#include <string>
#include <limits>
#include <vtkCell.h>
#ifndef MNT_UGRID_READER
#define MNT_UGRID_READER

#define NUM_PARAM_DIMS 2
#define NUM_SPACE_DIMS 3

class UgridReader {

public:

/**
 * Constructor
 */
UgridReader() {
}

/**
 * Destructor
 */
~UgridReader() {
}


/**
 * Get the number of faces
 * @return number
 */
size_t getNumberOfFaces() const {
    return this->numFaces;
}

/**
 * Get the number of edges
 * @return number
 */
size_t getNumberOfEdges() const {
    return this->numEdges;
}

/**
 * Get the number of points/vertices
 * @return number
 */
size_t getNumberOfPoints() const {
    return this->numPoints;
}


/**
 * Get pointer to the edge Ids
 * @param face Id 
 * @return pointer
 */
const size_t* getFaceEdgeIds(long long faceId) const;

/**
 * Get pointer to the point Ids
 * @param edge Id 
 * @return pointer
 */
const size_t* getEdgePointIds(long long edgeId) const;

/**
 * Get pointer to the coordinates
 * @param pointId point id
 * @return pointer
 */
const double* getPoint(long long pointId) const {
    return &this->points[pointId*NUM_SPACE_DIMS];
}

/**
 * Get min/max range of the domain
 * @param xmin low point of the domain (output)
 * @param xmax high point of the domain (output)
 */
void getRange(double xmin[], double xmax[]) const;


/**
 * Load from Ugrid file 
 * @param filename file name
 * @return error (0=OK)
 */
int load(const std::string& filename);

/**
 * Get the face vertex coordinates
 * @return array of points
 */
std::set< Vector<double> > getFacePoints(long long faceId) const;

/**
 * Get the edge vertex coordinates
 * @return array of points
 */
std::vector< Vector<double> > getEdgePoints(long long edgeId) const;


private:

    // face to edge connectivity
    std::vector<long long> face2Edges;

    // edge to node connectivity
    std::vector<long long> edge2Points;

    // vertex coordinates
    std::vector<double> points;

    // domain min/max
    Vector<double> xmin;
    Vector<double> xmax;

    size_t numPoints;
    size_t numEdges;
    size_t numFaces;

    int readPoints(int ncid);

    int findVariableIdWithAttribute(int ncid, const std::string& attrname, 
                                    const std::string& attrval, size_t* nsize);

};

#endif // MNT_UGRID_READER