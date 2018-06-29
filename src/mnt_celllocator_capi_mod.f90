module mnt_celllocator_capi_mod
  ! C function prototypes
  interface

    function mnt_celllocator_new(obj) bind(C)
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_double
      integer(c_size_t), intent(inout) :: obj
      integer(c_int)                   :: mnt_celllocator_new
    end function mnt_celllocator_new

    function mnt_celllocator_del(obj) bind(C)
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_double
      integer(c_size_t), intent(inout) :: obj
      integer(c_int)                   :: mnt_celllocator_del
    end function mnt_celllocator_del

    function mnt_celllocator_setpoints(obj, nverts_per_cell, ncells, verts) bind(C)
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_double
      integer(c_size_t), intent(inout)         :: obj
      integer(c_int), value                    :: nverts_per_cell
      integer(c_size_t), value                 :: ncells
      real(c_double), intent(in)               :: verts(*) ! const double*
      integer(c_int)                           :: mnt_celllocator_setpoints
    end function mnt_celllocator_setpoints

    function mnt_celllocator_build(obj) bind(C)
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_double
      integer(c_size_t), intent(inout) :: obj
      integer(c_int)                   :: mnt_celllocator_build
    end function mnt_celllocator_build

    function mnt_celllocator_find(obj, point, cell_id, pcoords) bind(C)
      use, intrinsic :: iso_c_binding, only: c_size_t, c_int, c_double
      integer(c_size_t), intent(inout)         :: obj
      real(c_double), intent(in)               :: point(*)   ! const double*
      integer(c_size_t), intent(out)           :: cell_id
      real(c_double), intent(out)              :: pcoords(*) ! double*
      integer(c_int)                           :: mnt_celllocator_find
    end function mnt_celllocator_find
  end interface
end module mnt_celllocator_capi_mod

