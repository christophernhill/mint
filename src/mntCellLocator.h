#include <vector>
#include <vtkUnstructuredGrid.h>
#include <vtkCellLocator.h>
#include <vtkGenericCell.h>
#include <mntGrid.h>

#ifndef MNT_CELL_LOCATOR
#define MNT_CELL_LOCATOR

/**
 * A class to compute the regridding weights of an edge-centred field
 */

struct CellLocator_t {
    Grid_t* grid;
    vtkCellLocator* loc;
    vtkGenericCell* cell;
};

/**
 * Constructor
 * @return error code (0 is OK)
 */
extern "C"
int mnt_celllocator_new(CellLocator_t** self);

/**
 * Destructor
 * @return error code (0 is OK)
 */
extern "C"
int mnt_celllocator_del(CellLocator_t** self);

/**
 * Set the points (vertices)
 * @param ncells number of cells
 * @param points flat array of size 4*ncells*3
 * @return error code (0 is OK)
 */
extern "C"
int mnt_celllocator_setpoints(CellLocator_t** self, int ncells, const double points[]);

/**
 * Build the regridder
 * @return error code (0 is OK)
 */
extern "C"
int mnt_celllocator_build(CellLocator_t** self);

/**
 * Find the cell and the parametric coordinates
 * @param point target point 
 * @param cellId cell Id (< 0 if not found)
 * @param pcoords parametric coordinates in the unit cell (filled in if found)
 * @return error code (0 is OK)
 */
extern "C"
int mnt_celllocator_find(CellLocator_t** self, const double point[], vtkIdType* cellId, double pcoords[]);


#endif // MNT_CELL_LOCATOR