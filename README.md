MINT - Mimetic Interpolation on the Sphere

## Overview

This project contains code to regrid edge centred fields from a source to a destination grid. The grid is stored as a collection of 
grid cells, which have four vertices (i.e. the cells are quadrilaterals). The edge field is stored as integrals of a vector field 
along each edge. The vertex coordinates are stored in longitude-latitude space.

The regridding method is mimetic in the sense that Stokes's theorem is satisfied to near machine precision. In particular, the 
loop integrals of an interpolated vector field deriving from a gradient is zero. 

## Prerequisites

You will need to have:

 * Python (2.7.14)
 * numpy (1.14.2)
 * netCDF4 (1.3.1)
 * VTK with python bindings enabled (8.1.0)

 We recommend to install the above packages using Anaconda.

## Example how to regrid from lat-lon to a cubed-sphere grid

In directory `tools/`:

 1. Generate lat-lon grid of Unified Model NetCDF flavour
 ```
 python latlon.py -nlon 100 -nlat 60 -o um100x60.nc
 ```

 2. Convert the destination grid to VTK file format
 ```
 python ugrid_reader.py -i ../data/cubedsphere10.nc -V cs.vtk
 ```

 3. Read the source grid, generate edge data and save the result as a VTK file
 ```
 python latlon_reader.py -i um100x60.nc -stream "x*cos(y)" -p 20 -V um100x60.vtk
 ```
 Note: `x*cos(y)` sets the stream function where x, y are the longitude, respectively, latitudes in radians. Argument -p 20 padds the grid on the high longitude 
 side by adding 20 cells. This is required to ensure that all destination grid cells are fully contained within source grid cells -
 padding should be applied when the destination grid has cells that cross the dateline. 


 4. Regrid the above field from the UM source grid to a cubed-sphere and save the result in a VTK file
 ```
 python regrid_edges.py -s um100x60.vtk -v "edge_integrated_velocity" -d cs.vtk -o regrid.vtk
 ```












