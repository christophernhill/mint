module mnt_regridedges_capi_mod
  ! C function prototypes
  interface

    function mnt_regridedges_new(obj) &
                                 bind(C, name='mnt_regridedges_new')
      ! Constructor
      ! @param obj instance of mntRegridEdges_t (opaque handle)
      ! @return 0 if successful
      use, intrinsic :: iso_c_binding, only: c_int, c_double, c_ptr
      implicit none
      type(c_ptr), intent(inout)       :: obj ! void**
      integer(c_int)                   :: mnt_regridedges_new
    end function mnt_regridedges_new

    function mnt_regridedges_del(obj) &
                                 bind(C, name='mnt_regridedges_del')
      ! Destructor
      ! @param obj instance of mntRegridEdges_t (opaque handle)
      ! @return 0 if successful
      use, intrinsic :: iso_c_binding, only: c_int, c_double, c_ptr
      implicit none
      type(c_ptr), intent(inout)       :: obj ! void**
      integer(c_int)                   :: mnt_regridedges_del
    end function mnt_regridedges_del

    function mnt_regridedges_loadSrcGrid(obj, filename, n) &
                                         bind(C, name='mnt_regridedges_loadSrcGrid')
      ! Load source grid from 2d UGRID file 
      ! @param obj instance of mntRegridEdges_t (opaque handle)
      ! @param filename file name
      ! @param n length of string filename
      ! @return 0 if sucecssful
      ! @note must invoke constructor prior to this call
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_ptr, c_char
      implicit none
      type(c_ptr), intent(inout)               :: obj ! void**
      character(kind=c_char), intent(in)       :: filename(*)
      integer(c_int), value                    :: n
      integer(c_int)                           :: mnt_regridedges_loadSrcGrid
    end function mnt_regridedges_loadSrcGrid

    function mnt_regridedges_loadDstGrid(obj, filename, n) &
                                         bind(C, name='mnt_regridedges_loadDstGrid')
      ! Load destination grid from 2d UGRID file 
      ! @param obj instance of mntRegridEdges_t (opaque handle)
      ! @param filename file name
      ! @param n length of string filename
      ! @return 0 if sucecssful
      ! @note must invoke constructor prior to this call
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_ptr, c_char
      implicit none
      type(c_ptr), intent(inout)               :: obj ! void**
      character(kind=c_char), intent(in)       :: filename(*)
      integer(c_int), value                    :: n
      integer(c_int)                           :: mnt_regridedges_loadDstGrid
    end function mnt_regridedges_loadDstGrid

    function mnt_regridedges_build(obj, num_cells_per_bucket) &
                                   bind(C, name='mnt_regridedges_build')
      ! Build locator object
      ! @param obj instance of mntregridedges_t (opaque handle)
      ! @param num_cells_per_bucket number of cells per bucket
      ! @return 0 if successful
      use, intrinsic :: iso_c_binding, only: c_int, c_double, c_ptr
      implicit none
      type(c_ptr), intent(inout)       :: obj ! void**
      integer(c_int), value            :: num_cells_per_bucket
      integer(c_int)                   :: mnt_regridedges_build
    end function mnt_regridedges_build

    function mnt_regridedges_loadWeights(obj, filename, n) & 
                                         bind(C, name='mnt_regridedges_loadWeights')
      ! Load interpolation weights from NetCDF file
      ! @param obj instance of mntregridedges_t (opaque handle)
      ! @param filename file name, \0 (char(0)) terminated
      ! @param n length of filename 
      ! @return 0 if successful
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_ptr, c_char
      implicit none
      type(c_ptr), intent(inout)               :: obj ! void**
      character(kind=c_char), intent(in)       :: filename(*)
      integer(c_int), value                    :: n
      integer(c_int)                           :: mnt_regridedges_loadWeights
    end function mnt_regridedges_loadWeights

    function mnt_regridedges_dumpWeights(obj, filename, n) & 
                                         bind(C, name='mnt_regridedges_dumpWeights')
      ! Dump interpolation weights in NetCDF file
      ! @param obj instance of mntregridedges_t (opaque handle)
      ! @param filename file name, \0 (char(0)) terminated
      ! @param n length of filename 
      ! @return 0 if successful
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_ptr, c_char
      implicit none
      type(c_ptr), intent(inout)               :: obj ! void**
      character(kind=c_char), intent(in)       :: filename(*)
      integer(c_int), value                    :: n
      integer(c_int)                           :: mnt_regridedges_dumpWeights
    end function mnt_regridedges_dumpWeights

    function mnt_regridedges_print(obj) & 
                                  bind(C, name='mnt_regridedges_print')
      ! Print interpolation weights
      ! @param obj instance of mntregridedges_t (opaque handle)
      ! @return 0 if successful
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_ptr, c_char
      implicit none
      type(c_ptr), intent(inout)               :: obj ! void**
      integer(c_int)                           :: mnt_regridedges_print   
    end function mnt_regridedges_print

    function mnt_regridedges_applyCellEdge(obj, src_data, dst_data) &
                                           bind(C, name='mnt_regridedges_applyCellEdge')
      ! Apply weights to cell by cell edge field
      ! @param obj instance of mntregridedges_t (opaque handle)
      ! @param src_data edge field defined cell by cell on the source grid (array of size 4*num_src_cells)
      ! @param dst_data edge field defined cell by cell on the destination grid (array of size 4*num_dst_cells)
      ! @return 0 if successful
      use, intrinsic :: iso_c_binding, only: c_int, c_double, c_ptr
      implicit none
      type(c_ptr), intent(inout)               :: obj ! void**
      real(c_double), intent(in)               :: src_data(*)
      real(c_double), intent(out)              :: dst_data(*)
      integer(c_int)                           :: mnt_regridedges_applyCellEdge
    end	function mnt_regridedges_applyCellEdge

    function mnt_regridedges_applyUniqueEdge(obj, src_data, dst_data) &
                                             bind(C, name='mnt_regridedges_applyUniqueEdge')
      ! Apply weights to unique cell Id edge field
      ! @param obj instance of mntregridedges_t (opaque handle)
      ! @param src_data source field defined for each unique edge Id
      ! @param dst_data destination field defined for each for each unique edge Id
      ! @return 0 if successful
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_double, c_ptr
      implicit none
      type(c_ptr), intent(inout)               :: obj ! void**
      real(c_double), intent(in)               :: src_data(*)
      real(c_double), intent(out)              :: dst_data(*)
      integer(c_int)                           :: mnt_regridedges_applyUniqueEdge
    end	function mnt_regridedges_applyUniqueEdge

  end interface

end module mnt_regridedges_capi_mod

