include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_Fortran_MODULE_DIRECTORY}
)

configure_file(testGrid.cpp testGrid.cxx)
configure_file(testRegridEdgesFromUgrid.cpp testRegridEdgesFromUgrid.cxx)

add_executable(testSimpleRegridEdges testSimpleRegridEdges.cxx)
target_link_libraries(testSimpleRegridEdges
                    mint
                    ${VTK_LIBRARIES}
                    ${NETCDF_LIBRARIES}
)

add_executable(testRegridEdgesFromUgrid testRegridEdgesFromUgrid.cxx)
target_link_libraries(testRegridEdgesFromUgrid
                    mint
                    ${VTK_LIBRARIES}
                    ${NETCDF_LIBRARIES}
)

add_executable(testPolylineParser testPolylineParser.cxx)
target_link_libraries(testPolylineParser
                    mint
                    ${VTK_LIBRARIES}
)

add_executable(testLineGridIntersector testLineGridIntersector.cxx)
target_link_libraries(testLineGridIntersector
                    mint
                    ${VTK_LIBRARIES}
)

add_executable(testFindCellsAlongLine testFindCellsAlongLine.cxx)
target_link_libraries(testFindCellsAlongLine
                    mint
                    ${VTK_LIBRARIES}
)

add_executable(testLineLineIntersector testLineLineIntersector.cxx)
target_link_libraries(testLineLineIntersector
                    mint
                    ${VTK_LIBRARIES}
)

add_executable(testLineTriangleIntersector testLineTriangleIntersector.cxx)
target_link_libraries(testLineTriangleIntersector
                    mint
                    ${VTK_LIBRARIES}
)

add_executable(testPolysegmentIter testPolysegmentIter.cxx)
target_link_libraries(testPolysegmentIter
                      mint
                      ${VTK_LIBRARIES}
)

add_executable(testPolysegmentIter3d testPolysegmentIter3d.cxx)
target_link_libraries(testPolysegmentIter3d
                      mint
                      ${VTK_LIBRARIES}
)

add_executable(testGrid testGrid.cxx)
target_link_libraries(testGrid
                      mint
                      ${VTK_LIBRARIES}
                      ${NETCDF_LIBRARIES}
)

add_executable(testCellLocator testCellLocator.cxx)
target_link_libraries(testCellLocator
                      mint
                      ${VTK_LIBRARIES}
                      ${NETCDF_LIBRARIES}
)

add_executable(testPrintAddressF testPrintAddressF.f90)
set_target_properties(testPrintAddressF PROPERTIES LINKER_LANGUAGE Fortran)
target_link_libraries(testPrintAddressF
                      mint
                      ${VTK_LIBRARIES}
                      ${NETCDF_LIBRARIES}
)

add_executable(testCellLocatorF testCellLocatorF.f90)
set_target_properties(testCellLocatorF PROPERTIES LINKER_LANGUAGE Fortran)
target_link_libraries(testCellLocatorF
                      mint
                      ${VTK_LIBRARIES}
                      ${NETCDF_LIBRARIES}
)

add_executable(testCellLocatorFromFileF testCellLocatorFromFileF.f90)
set_target_properties(testCellLocatorFromFileF PROPERTIES LINKER_LANGUAGE Fortran)
target_link_libraries(testCellLocatorFromFileF
                      mint
                      ${VTK_LIBRARIES}
                      ${NETCDF_LIBRARIES}
)

add_executable(testC2fString testC2fString.f90)
set_target_properties(testC2fString PROPERTIES LINKER_LANGUAGE Fortran)
target_link_libraries(testC2fString
                      mint
                      ${VTK_LIBRARIES}
)


add_executable(testExprParser testExprParser.cxx)
target_link_libraries(testExprParser
                      mint
                      ${VTK_LIBRARIES}
)

add_executable(testSize testSize.cxx)
target_link_libraries(testSize
                      mint
                      ${VTK_LIBRARIES}
)

add_executable(testCmdLineArgParserC testCmdLineArgParserC.cxx)
target_link_libraries(testCmdLineArgParserC
                      mint
                      ${VTK_LIBRARIES}
)

add_executable(testRegridEdgesF testRegridEdgesF.f90)
set_target_properties(testRegridEdgesF PROPERTIES LINKER_LANGUAGE Fortran)
target_link_libraries(testRegridEdgesF
                    mint
                    ${VTK_LIBRARIES}
                    ${NETCDF_LIBRARIES}
)

add_executable(testCmdLineArgParserF testCmdLineArgParserF.f90)
set_target_properties(testCmdLineArgParserF PROPERTIES LINKER_LANGUAGE Fortran)
target_link_libraries(testCmdLineArgParserF
                      mint
                      ${VTK_LIBRARIES}
)

add_executable(testCmdLineArgParserFB testCmdLineArgParserFB.f90)
set_target_properties(testCmdLineArgParserFB PROPERTIES LINKER_LANGUAGE Fortran)
target_link_libraries(testCmdLineArgParserFB
                      mint
                      ${VTK_LIBRARIES}
)

add_test(NAME regridEdgesF
	     COMMAND testRegridEdgesF 
	     "-s" "${CMAKE_SOURCE_DIR}/data/cs_16.nc" 
	     "-d" "${CMAKE_SOURCE_DIR}/data/cs_4.nc" 
	     "-o" "weights.nc")
set_tests_properties(regridEdgesF PROPERTIES
                     FAIL_REGULAR_EXPRESSION "ERROR")

add_test(NAME simpleRegridEdges COMMAND testSimpleRegridEdges)
add_test(NAME regridEdgesFromUgrid COMMAND testRegridEdgesFromUgrid)
add_test(NAME polylineParser COMMAND testPolylineParser)
add_test(NAME lineGridIntersector COMMAND testLineGridIntersector)
add_test(NAME findCellsAlongLine COMMAND testFindCellsAlongLine)
add_test(NAME lineLineIntersector COMMAND testLineLineIntersector)
add_test(NAME lineTriangleIntersector COMMAND testLineTriangleIntersector)
add_test(NAME polysegmentIter COMMAND testPolysegmentIter)
add_test(NAME polysegmentIter3d COMMAND testPolysegmentIter3d)
add_test(NAME grid COMMAND testGrid)
add_test(NAME cellLocator COMMAND testCellLocator)
add_test(NAME cellLocatorF COMMAND testCellLocatorF)
add_test(NAME cellLocatorFromFile_cs_64 COMMAND testCellLocatorFromFileF "-v" "-i" "${CMAKE_SOURCE_DIR}/data/cs_64.vtk" "-n" "10" "-o" "out.vtk")
add_test(NAME cellLocatorFromFile_lfric_24576cells COMMAND testCellLocatorFromFileF "-i" "${CMAKE_SOURCE_DIR}/data/lfric_grid.vtk" "-n" "256")
add_test(NAME cellLocatorFromFile_lfric_24576cells_check COMMAND testCellLocatorFromFileF "-i" "${CMAKE_SOURCE_DIR}/data/lfric_grid.vtk" "-n" "256" "-c")
add_test(NAME exprParser COMMAND testExprParser)
add_test(NAME size COMMAND testSize)
add_test(NAME cmdLineArgParserC COMMAND testCmdLineArgParserC)
add_test(NAME cmdLineArgParserC2 
        COMMAND testCmdLineArgParserC "-i" "-234" "-d" -2.34 "-s" "hi_there")
add_test(NAME cmdLineArgParserF COMMAND testCmdLineArgParserF)
add_test(NAME cmdLineArgParserF2 
         COMMAND testCmdLineArgParserF "-i" "input.vtk" "-o" "output.vtk" "-d" "-2.34" "-n" "100" "-v")
add_test(NAME cmdLineArgParserFB
         COMMAND testCmdLineArgParserFB "-i" "in.vtk")
add_test(NAME testPrintAddressF
         COMMAND testPrintAddressF)
add_test(NAME c2fString
         COMMAND testC2fString)

add_test(NAME regrid_edgesVTK16
         COMMAND "${CMAKE_BINARY_DIR}/tools/regrid_edges" 
                 "-s" "${CMAKE_SOURCE_DIR}/data/um100x60.vtk" 
                 "-v" "edge_integrated_velocity" 
                 "-d" "${CMAKE_SOURCE_DIR}/data/cs_16.vtk"
                 "-o" "regrid_edges_output.vtk")
# make sure the max loop integral is very small
set_tests_properties(regrid_edgesVTK16 PROPERTIES
                     PASS_REGULAR_EXPRESSION "Min/avg/max cell loop integrals: [^/]*/[^/]*/[^e]+e-1[0-9]")

add_test(NAME regrid_edgesUgrid16To4
         COMMAND "${CMAKE_BINARY_DIR}/tools/regrid_edges" 
                 "-s" "${CMAKE_SOURCE_DIR}/data/cs_16.nc" 
                 "-v" "edge_integrated_velocity" 
                 "-d" "${CMAKE_SOURCE_DIR}/data/cs_4.nc"
                 "-o" "regrid_edges_output_Ugrid16To4.vtk")
#set_tests_properties(regrid_edgesUgrid16To4 PROPERTIES
#                     PASS_REGULAR_EXPRESSION "Min/avg/max cell loop integrals: [^/]*/[^/]*/[^e]+e-1[0-9]")


add_test(NAME regrid_edges64
         COMMAND "${CMAKE_BINARY_DIR}/tools/regrid_edges" 
                 "-s" "${CMAKE_SOURCE_DIR}/data/um100x60.vtk" 
                 "-v" "edge_integrated_velocity" 
                 "-d" "${CMAKE_SOURCE_DIR}/data/cs_64.vtk"
                 "-o" "regrid_edges_output.vtk")
# make sure the max loop integral is very small
set_tests_properties(regrid_edges64 PROPERTIES
                     PASS_REGULAR_EXPRESSION "Min/avg/max cell loop integrals: [^/]*/[^/]*/[^e]+e-1[0-9]")

