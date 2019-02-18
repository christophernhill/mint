#include <vector>
#include <string>
#include <vtkCell.h>
#ifndef MNT_UGRID_EDGE_READER
#define MNT_UGRID_EDGE_READER

class UgridEdgeReader {

public:

/**
 * Constructor
 */
UgridEdgeReader();

/**
 * Destructor
 */
~UgridEdgeReader();


/**
 * Get the number of edges
 * @return number
 */
size_t getNumberOfEdges() const;


/**
 * Get edge
 * @param edge Id 
 * @param pBeg start point of the edge (output)
 * @param pEnd end point of the edge (output)
 */
void getEdge(size_t edgeId, double pBeg[], double pEnd[]) const;


/**
 * Load from Ugrid file 
 * @param filename file name
 * @return error (0=OK)
 */
int load(const std::string& filename);


private:

	std::vector<double> readPoints(int ncid);

	int readEdgeConnectivity(int ncid, const std::vector<double>& points);

	int findVariableIdWithCfRole(int ncid, const std::string& cf_role, int* ndims, int dimids[]);

	int findVariableIdWithStandardName(int ncid, const std::string& standard_name, int* ndims, int dimids[]);


    // edge to node connectivity
    std::vector<double> edge2Points;
};

#endif // MNT_UGRID_EDGE_READER