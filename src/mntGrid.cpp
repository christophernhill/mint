#include <mntGrid.h>
#include <vtkCellData.h>
#include <vtkUnstructuredGridWriter.h>
#include <fstream>
#include <netcdf.h>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>

#define LON_INDEX 0
#define LAT_INDEX 1
#define ELV_INDEX 2

extern "C" 
int mnt_grid_new(Grid_t** self) {

    *self = new Grid_t();
    (*self)->pointData = NULL;
    (*self)->points = NULL;
    (*self)->grid = NULL;
    (*self)->reader = NULL;
    (*self)->doubleArrays.resize(0);
    return 0;
}

extern "C"
int mnt_grid_del(Grid_t** self) {

    for (size_t i = 0; i < (*self)->doubleArrays.size(); ++i) {
        (*self)->doubleArrays[i]->Delete();
    }
    if ((*self)->reader) {
        (*self)->reader->Delete();
    }
    else {
        if ((*self)->grid) (*self)->grid->Delete();
    }
    if ((*self)->points) (*self)->points->Delete();
    if ((*self)->pointData) (*self)->pointData->Delete();

    delete *self;

    return 0;
}

extern "C"
int mnt_grid_setPointsPtr(Grid_t** self, int nVertsPerCell, 
	                      vtkIdType ncells, const double points[]) {

    (*self)->pointData = vtkDoubleArray::New();
    (*self)->points = vtkPoints::New();
    (*self)->grid = vtkUnstructuredGrid::New();

    int save = 1;
    int npoints = nVertsPerCell * ncells;
    (*self)->pointData->SetNumberOfTuples(npoints);
    (*self)->pointData->SetNumberOfComponents(3);
    (*self)->pointData->SetVoidArray((double*) points, npoints*3, save);

    (*self)->points->SetData((*self)->pointData);

    (*self)->grid->Allocate(ncells, 1);

    int cellType = -1;
    if (nVertsPerCell == 4) {
        cellType = VTK_QUAD;
    }
    else if (nVertsPerCell == 8) {
        cellType = VTK_HEXAHEDRON;
    }
    else {
        // error
        return 1;
    }

    // connect
    vtkIdList* ptIds = vtkIdList::New();

    ptIds->SetNumberOfIds(nVertsPerCell);
    for (int i = 0; i < ncells; ++i) {
        for (int j = 0; j < nVertsPerCell; ++j) {
            ptIds->SetId(j, nVertsPerCell*i + j);
        }
        (*self)->grid->InsertNextCell(cellType, ptIds);
    }
    (*self)->grid->SetPoints((*self)->points);
    (*self)->grid->BuildLinks(); // DO WE NEED THIS?

    // clean
    ptIds->Delete();

    return 0;
}

extern "C"
int mnt_grid_attach(Grid_t** self, const char* varname, int nDataPerCell, const double data[]) {

    if (!(*self)->grid) {
        return 1;
    }

    vtkIdType ncells = (*self)->grid->GetNumberOfCells();

    vtkDoubleArray* vtkdata = vtkDoubleArray::New();
    vtkdata->SetName(varname);
    vtkdata->SetNumberOfTuples(ncells);
    vtkdata->SetNumberOfComponents(nDataPerCell);
    int save = 1;
    vtkdata->SetVoidArray((double*) data, ncells*nDataPerCell, save);

    // store
    (*self)->doubleArrays.push_back(vtkdata);

    // add to the grid
    (*self)->grid->GetCellData()->AddArray(vtkdata);

    return 0;
}


extern "C"
int mnt_grid_get(Grid_t** self, vtkUnstructuredGrid** grid_ptr) {
    *grid_ptr = (*self)->grid;
    return 0;
}

extern "C"
int mnt_grid_loadFrom2DUgrid(Grid_t** self, const char* filename) {

    // open the file
    int ncid;
    int ier = nc_open(filename, NC_NOWRITE, &ncid);
    if (ier != NC_NOERR) {
        std::cerr << "ERROR: cannot open \"" << filename << "\"\n";
        return 1;
    }

    size_t ncells = 0;
    size_t nedges = 0;
    size_t numVertsPerCell = 0;
    size_t numEdgesPerCell = 0;
    size_t numVertsPerEdge = 0;

    std::vector<double> lats;
    std::vector<double> lons;

    // get the number of variables
    int nvars;
    ier = nc_inq_nvars(ncid, &nvars);
    if (ier != NC_NOERR) {
        std::cerr << "ERROR: after inquiring the number of variables (ier = " 
                  << ier << ")\n";
        return 1;
    }

    //
    // find the latitudes, longitudes and cell connectivity
    //

    // allocate space for the variable name
    char varname[NC_MAX_NAME];

    // allocate and fill in with ' '
    std::string standard_name(NC_MAX_NAME, ' ');
    std::string long_name(NC_MAX_NAME, ' ');
    std::string cf_role(NC_MAX_NAME, ' ');
    std::string units(NC_MAX_NAME, ' ');
    nc_type xtype;
    int ndims;
    int natts;
    int startIndex = 0;

    // iterate over the variables in the netcdf file
    for (int ivar = 0; ivar < nvars; ++ivar) {

        // get the number of dimensions of this variable
        ier = nc_inq_varndims(ncid, ivar, &ndims);
        if (ier != NC_NOERR) {
            std::cerr << "ERROR: after inquiring the number of dimensions for var = " 
                      << ivar << " (ier = " << ier << ")\n";
            return 2;
        }

        // get each of the dimensions of this variable
        std::vector<int> dimids(ndims);

        ier = nc_inq_var(ncid, ivar, varname, &xtype, &ndims, &dimids[0], &natts);

        // reset
        standard_name.assign(NC_MAX_NAME, ' ');
        long_name.assign(NC_MAX_NAME, ' ');
        cf_role.assign(NC_MAX_NAME, ' ');

        // get the variable attributes
        int ier1 = nc_get_att_text(ncid, ivar, "standard_name", &standard_name[0]);
        int ier2 = nc_get_att_text(ncid, ivar, "long_name", &long_name[0]);
        int ier3 = nc_get_att_text(ncid, ivar, "cf_role", &cf_role[0]);
        int ier4 = nc_get_att_int(ncid, ivar, "start_index", &startIndex);
        int ier5 = nc_get_att_text(ncid, ivar, "units", &units[0]);

        if (ier1 == NC_NOERR && ier2 == NC_NOERR) {

            // variable has "standard_name" and long_name attributes

            // get the number of elements
            int nelems = 1;
            for (int i = 0; i < ndims; ++i) {
                size_t dim;
                ier = nc_inq_dimlen(ncid, dimids[i], &dim);
                if (ier != NC_NOERR) {
                    std::cerr << "ERROR: after getting the dimension size (ier = " << ier << ")\n";
                        return 1;
                }
                nelems *= dim;
            }

            // is it a latitude or a longitude, defined on nodes?

            if (standard_name.find("latitude") != std::string::npos &&
                long_name.find("node") != std::string::npos) {

                // allocate the data to receive the lats
                lats.resize(nelems);

                // read the latitudes as doubles, netcdf will convert if stored as floats
                ier = nc_get_var_double(ncid, ivar, &lats[0]);
                if (ier != NC_NOERR) {
                    std::cerr << "ERROR: after reading latitudes (ier = " << ier << ")\n";
                    return 1;
                }
            }
            else if (standard_name.find("longitude") != std::string::npos && 
                     long_name.find("node") != std::string::npos) {

                // allocate to receive the lons
                lons.resize(nelems);

                // read the longitudes as doubles, netcdf will convert if stored as floats
                ier = nc_get_var_double(ncid, ivar, &lons[0]);
                if (ier != NC_NOERR) {
                    std::cerr << "ERROR: after reading longitudes (ier = " << ier << ")\n";
                    return 1;
                }
            }
        }
        else if (ier3 == NC_NOERR && ier4 == NC_NOERR &&
                 cf_role.find("face_node_connectivity") != std::string::npos) {

            // get the number of cells
            ier = nc_inq_dimlen(ncid, dimids[0], &ncells);
            // get the number of vertices per cell
            ier = nc_inq_dimlen(ncid, dimids[1], &numVertsPerCell);
            size_t nelems = ncells * numVertsPerCell;

            // allocate the data
            (*self)->faceNodeConnectivity.resize(nelems);

            // read the connectivity
            ier = nc_get_var_longlong(ncid, ivar, &(*self)->faceNodeConnectivity[0]);
            if (ier != NC_NOERR) {
                std::cerr << "ERROR: after reading face-node connectivity (ier = " << ier << ")\n";
                return 1;
            }

            // substract start index
            for (size_t i = 0; i < nelems; ++i) {
                (*self)->faceNodeConnectivity[i] -= startIndex;
            }

        }
        else if (ier3 == NC_NOERR && ier4 == NC_NOERR &&
                 cf_role.find("face_edge_connectivity") != std::string::npos) {

            // get the number of cells
            ier = nc_inq_dimlen(ncid, dimids[0], &ncells);
            // get the number of edges per cell
            ier = nc_inq_dimlen(ncid, dimids[1], &numEdgesPerCell);
            size_t nelems = ncells * numEdgesPerCell;

            // allocate the data
            (*self)->faceEdgeConnectivity.resize(nelems);

            // read the connectivity
            ier = nc_get_var_longlong(ncid, ivar, &(*self)->faceEdgeConnectivity[0]);
            if (ier != NC_NOERR) {
                std::cerr << "ERROR: after reading face-edge connectivity (ier = " << ier << ")\n";
                return 1;
            }

            // substract start index
            for (size_t i = 0; i < nelems; ++i) {
                (*self)->faceEdgeConnectivity[i] -= startIndex;
            }

        }
        else if (ier3 == NC_NOERR && ier4 == NC_NOERR &&
                 cf_role.find("edge_node_connectivity") != std::string::npos) {

            // get the number of edges
            ier = nc_inq_dimlen(ncid, dimids[0], &nedges);
            // get the number of edges per cell
            ier = nc_inq_dimlen(ncid, dimids[1], &numVertsPerEdge);
            size_t nelems = nedges * numVertsPerEdge;

            // allocate the data
            (*self)->edgeNodeConnectivity.resize(nelems);

            // read the connectivity
            ier = nc_get_var_longlong(ncid, ivar, &(*self)->edgeNodeConnectivity[0]);
            if (ier != NC_NOERR) {
                std::cerr << "ERROR: after reading face-edge connectivity (ier = " << ier << ")\n";
                return 1;
            }

            // substract start index
            for (size_t i = 0; i < nelems; ++i) {
                (*self)->edgeNodeConnectivity[i] -= startIndex;
            }

        }

    }

    // close the netcdf file
    ier = nc_close(ncid);

    // repackage the cell vertices as a flat array 

    if (lons.size() > 0 && lats.size() > 0 && (*self)->faceNodeConnectivity.size() > 0) {

        // allocate the vertices and set the values
        (*self)->verts.resize(ncells * numVertsPerCell * 3);

        std::vector<double> diffLonMinusZeroPlus(3);

        for (size_t icell = 0; icell < ncells; ++icell) {

            // fix longitude when crossing the dateline
            // use the first longitude as the base
            size_t kBase = (*self)->faceNodeConnectivity[icell*numVertsPerCell];
            double lonBase = lons[kBase];

            int poleNode = -1;
            for (int node = 0; node < numVertsPerCell; ++node) {

                size_t k = (*self)->faceNodeConnectivity[icell*numVertsPerCell + node];
                double lon = lons[k];

                // add/subtract 360.0, whatever it takes to reduce the distance 
                // between this longitude and the base longitude
                double diffLon = lon - lonBase;
                diffLonMinusZeroPlus[0] = std::abs(diffLon - 360.);
                diffLonMinusZeroPlus[1] = std::abs(diffLon - 0.);
                diffLonMinusZeroPlus[2] = std::abs(diffLon + 360.);
                std::vector<double>::iterator it = std::min_element(diffLonMinusZeroPlus.begin(), 
                                                                    diffLonMinusZeroPlus.end());
                int indexMin = (int) std::distance(diffLonMinusZeroPlus.begin(), it);

                // fix the longitude
                lon += (indexMin - 1) * 360.0;

                if (std::abs(lats[k]) == 90.0) {
                    poleNode  = node;
                }

                // even in 2d we have three components
                (*self)->verts[LON_INDEX + node*3 + icell*numVertsPerCell*3] = lon;
                (*self)->verts[LAT_INDEX + node*3 + icell*numVertsPerCell*3] = lats[k];
                (*self)->verts[ELV_INDEX + node*3 + icell*numVertsPerCell*3] = 0.0;
            }

            // check if there if one of the cell nodes is at the north/south pole. In 
            // this case the longitude is ill-defined. Set the longitude there to the
            // average of the 3 other longitudes.

            if (poleNode >= 0) {
                double lonPole = 0;
                for (size_t i = poleNode + 1; i < poleNode + numVertsPerCell; ++i) {
                    size_t node = i % numVertsPerCell;
                    lonPole += (*self)->verts[0 + node*3 + icell*numVertsPerCell*3];
                }
                lonPole /= (double) (numVertsPerCell - 1);
                (*self)->verts[LON_INDEX + poleNode*3 + icell*numVertsPerCell*3] = lonPole;
            }
        }
    }

    // set the pointer
    ier = mnt_grid_setPointsPtr(self, (int) numVertsPerCell, (vtkIdType) ncells, 
    	                        &((*self)->verts[0]));

    return 0;
}

extern "C"
int mnt_grid_load(Grid_t** self, const char* filename) {
    // check if the file exists
    if (!fstream(filename).good()) {
        std::cerr << "ERROR file " << filename << " does not exist\n";
        return 1;        
    }

    if ((*self)->grid) {
        (*self)->grid->Delete();
    }
    (*self)->reader = vtkUnstructuredGridReader::New();
    (*self)->reader->SetFileName(filename);
    (*self)->reader->Update();
    (*self)->grid = (*self)->reader->GetOutput();
    return 0;
}

extern "C"
int mnt_grid_dump(Grid_t** self, const char* filename) {
    vtkUnstructuredGridWriter* writer = vtkUnstructuredGridWriter::New();
    writer->SetFileName(filename);
    writer->SetInputData((*self)->grid);
    writer->Update();
    writer->Delete();
    return 0;
}

extern "C"
int mnt_grid_print(Grid_t** self) {

    vtkPoints* points = (*self)->grid->GetPoints();
    vtkIdType npoints = points->GetNumberOfPoints();
    std::cerr << "Number of points: " << npoints << '\n';

    vtkIdType ncells = (*self)->grid->GetNumberOfCells();
    std::cerr << "Number of cells: " << ncells << '\n';

    std::vector<double> pt(3);

    for (vtkIdType i = 0; i < ncells; ++i) {

        vtkCell* cell = (*self)->grid->GetCell(i);

        for (int j = 0; j < cell->GetNumberOfPoints(); ++j) {
            vtkIdType k = cell->GetPointId(j);
            (*self)->points->GetPoint(k, &pt[0]);
            std::cout << "\tpoint " << pt[0] << ',' << pt[1] << ',' << pt[2] << '\n';
        }
    }

    return 0;
}

extern "C"
int mnt_grid_getPoints(Grid_t** self, vtkIdType cellId, int edgeIndex,
                       double point0[], double point1[]) {

    // flat index for the start point, 4 points per cell, 3d coordinates
    size_t k0 = 4*3*cellId + 3*((edgeIndex + 0) % 4);

    // flat index for the end point, 4 points per cell, 3d coordinates
    size_t k1 = 4*3*cellId + 3*((edgeIndex + 1) % 4);

    if (edgeIndex < 2) {
        // edge's direction is counterclockwise
        for (size_t i = 0; i < 3; ++i) {
            point0[i] = (*self)->verts[i + k0];
            point1[i] = (*self)->verts[i + k1];
        }
    }
    else {
        // edge's direction is clockwise - reverse order of point0 and point1
        for (size_t i = 0; i < 3; ++i) {
            point1[i] = (*self)->verts[i + k0];
            point0[i] = (*self)->verts[i + k1];
        }        
    } 

    return 0;
}

extern "C" 
int mnt_grid_getNodeIds(Grid_t** self, vtkIdType cellId, int edgeIndex, vtkIdType nodeIds[]) {
    
    // nodeIndex0,1 are the local cell indices of the vertices in the range 0-3
    int nodeIndex0 = edgeIndex;
    // 4 vertices per cell
    int nodeIndex1 = (edgeIndex + 1) % 4;

    // edges 2-3 go clockwise
    // edges 0-1 go anticlockwise
    if (edgeIndex >= 2) {
        // swap order
        int tmp = nodeIndex0;
        nodeIndex0 = nodeIndex1;
        nodeIndex1 = tmp;
    }

    nodeIds[0] = (*self)->faceNodeConnectivity[4*cellId + nodeIndex0];
    nodeIds[1] = (*self)->faceNodeConnectivity[4*cellId + nodeIndex1];

    return 0;
}

extern "C" 
int mnt_grid_getEdgeId(Grid_t** self, vtkIdType cellId, int edgeIndex, 
                       vtkIdType* edgeId, int* signEdge) {

    // initialize
    *edgeId = -1;
    *signEdge = 0;

    // fetch the node Ids of this edge
    vtkIdType nodeIds[2];
    int ier = mnt_grid_getNodeIds(self, cellId, edgeIndex, nodeIds);

    // iterate over the edges of this face until we find the edge
    // that has vertices nodeIds (but not necessarily in the same
    // order)
    for (int ie = 0; ie < 4; ++ie) {

        // edgeId under consideration
        vtkIdType eId = (*self)->faceEdgeConnectivity[4*cellId + ie];

        // vertex Ids of the edge
        vtkIdType nId0 = (*self)->edgeNodeConnectivity[eId*2 + 0];
        vtkIdType nId1 = (*self)->edgeNodeConnectivity[eId*2 + 1];

        if (nId0 == nodeIds[0] && nId1 == nodeIds[1]) {
            // found edge and the direction is left->right, bottom -> up
            *signEdge = 1;
            *edgeId = eId;
            break;
        }
        else if (nId0 == nodeIds[1] && nId1 == nodeIds[0]) {
            // found edge and the direction is opposite
            *signEdge = -1;
            *edgeId = eId;
            break;
        }
    }

    return 0;
}


extern "C"
int mnt_grid_getNumberOfCells(Grid_t** self, size_t* numCells) {

    *numCells = (*self)->grid->GetNumberOfCells();
    return 0;
}

int mnt_grid_getNumberOfUniqueEdges(Grid_t** self, size_t* numEdges) {
    
    *numEdges = (*self)->edgeNodeConnectivity.size() / 2;
    return 0;
}
