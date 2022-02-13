program main
   use, intrinsic::iso_fortran_env, only: DP => REAL64, input_unit, error_unit
   implicit none

   real(kind=DP) :: x1, y1, x2, y2

   integer :: io_stat
   character(len=1024) :: io_msg

   do while (.true.)
      read (unit=input_unit, fmt=*, iostat=io_stat, iomsg=io_msg) x1, y1, x2, y2

      if (io_stat < 0) exit

      if (io_stat > 0) then
         write (unit=error_unit, fmt="(2A)") &
            "Erreur I/O: ", io_msg
         stop 1
      end if

      print *, distance(x1, y1, x2, y2)
   end do
contains
   function distance(x1, y1, x2, y2) result(d)
      implicit none

      real(kind=DP), intent(in) :: x1, y1, x2, y2
      real(kind=DP) :: d

      d = sqrt((x1 - x2)**2 + (y1 - y2)**2)
   end function distance
end program main
