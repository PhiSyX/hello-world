program main
   use, intrinsic::iso_fortran_env, only: DP => REAL64, input_unit, error_unit
   implicit none

   real(kind=DP) :: min_val, max_val
   real(kind=DP) :: val

   integer :: io_stat
   character(len=1024) :: io_msg

   call get_limits(min_val, max_val)

   do
      read (unit=input_unit, fmt=*, iostat=io_stat, iomsg=io_msg) val
      if (io_stat < 0) exit
      call check_status(io_stat, io_msg)
      call clamp(val, min_val, max_val)
      print *, val
   end do
contains
   subroutine check_status(io_stat, io_msg)
      use, intrinsic::iso_fortran_env, only: error_unit
      implicit none

      integer, value :: io_stat
      character(len=*), intent(in) :: io_msg

      if (io_stat > 0) then
         write (unit=error_unit, fmt="(2A)") &
            "Erreur I/O: ", trim(io_msg)
         stop 1
      end if
   end subroutine check_status

   subroutine get_limits(min_val, max_val)
      implicit none

      real(kind=DP), intent(out) :: min_val, max_val

      character(len=1024) :: buffer
      integer :: io_stat
      character(len=1024) :: io_msg

      if (command_argument_count() == 0) then
         max_val = 1.0_DP
      else if (command_argument_count() >= 1) then
         call get_command_argument(1, buffer)
         read (buffer, fmt=*, iostat=io_stat, iomsg=io_msg) max_val
         call check_status(io_stat, io_msg)
      end if

      if (command_argument_count() == 2) then
         min_val = max_val
         call get_command_argument(2, buffer)
         read (buffer, fmt=*, iostat=io_stat, iomsg=io_msg) max_val
         call check_status(io_stat, io_msg)
      else
         min_val = -max_val
      end if
   end subroutine get_limits

   subroutine clamp(val, min_val, max_val)
      implicit none

      real(kind=DP), intent(inout) :: val
      real(kind=DP), intent(in) :: min_val, max_val

      if (val < min_val) then
         val = min_val
      else if (max_val < val) then
         val = max_val
      end if
   end subroutine clamp
end program main
