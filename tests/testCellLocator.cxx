#include <mntCellLocator.h>
#include <cassert>
#undef NDEBUG // turn on asserts

void test1Quad() {
    CellLocator_t* cloc;
    mnt_celllocator_new(&cloc);
    int num_cells = 1;
    int num_verts_per_cell = 4; // quad
    int num_points = num_cells * num_verts_per_cell;
    const double verts[] = {0., 0., 0.,
                            1., 0., 0.,
                            1., 1., 0.,
                            0., 1., 0.};
    mnt_celllocator_setPointsPtr(&cloc, num_verts_per_cell, num_cells, verts);
    int numCellsPerBucket = 512;
    mnt_celllocator_build(&cloc, numCellsPerBucket);
    const double target_point[] = {0.2, 0.3, 0.};
    double pcoords[3];
    long long cell_id;
    mnt_celllocator_find(&cloc, target_point, &cell_id, pcoords);
    std::cout << "test1Quad: cell id: " << cell_id << " pcoords = ";
    for (size_t i = 0; i < 3; ++i) std::cout << pcoords[i] << ',';
   	std::cout << '\n';
    assert(cell_id == 0);


    mnt_celllocator_del(&cloc);
}

void test1Hex() {
    CellLocator_t* cloc;
    mnt_celllocator_new(&cloc);
    int num_cells = 1;
    int num_verts_per_cell = 8; // hex
    int num_points = num_cells * num_verts_per_cell;
    const double verts[] = {0., 0., 0.,
                            1., 0., 0.,
                            1., 1., 0.,
                            0., 1., 0., 
                            0., 0., 1.,
                            1., 0., 1.,
                            1., 1., 1.,
                            0., 1., 1.};
    mnt_celllocator_setPointsPtr(&cloc, num_verts_per_cell, num_cells, verts);
    int numCellsPerBucket = 512;
    mnt_celllocator_build(&cloc, numCellsPerBucket);
    std::string outfile = std::string("testHex1.vtk");
    mnt_celllocator_dumpGrid(&cloc, outfile.c_str(), outfile.size());
    const double target_point[] = {0.2, 0.3, 0.4};
    double pcoords[3];
    long long cell_id;
    mnt_celllocator_find(&cloc, target_point, &cell_id, pcoords);
    std::cout << "test1Hex: cell id: " << cell_id << " pcoords = ";
    for (size_t i = 0; i < 3; ++i) std::cout << pcoords[i] << ',';
   	std::cout << '\n';
    assert(cell_id == 0);


    mnt_celllocator_del(&cloc);
}


int main(int argc, char** argv) {

    test1Quad();
    test1Hex();

    return 0;
}
