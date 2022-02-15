program main
   use, intrinsic::iso_fortran_env, only: DP => REAL64, I8 => INT64
   implicit none

   integer(kind=I8) :: i, total_iters
   real(kind=DP) :: delta, x, pi_value

   pi_value = 0.0_DP
   total_iters = get_total_iters()
   delta = 1.0_DP/total_iters
   x = 0.0_DP

   do i = 1, total_iters
      pi_value = pi_value + sqrt(1.0_DP - x**2)
      x = x + delta
   end do

   pi_value = 4.0_DP*pi_value/total_iters
   print '(F25.15)', pi_value
contains
   function get_total_iters() result(total_iters)
      use, intrinsic::iso_fortran_env, only: error_unit
      implicit none

      integer(kind=I8) :: total_iters
      integer(kind=I8), parameter :: default_total_iters = 1000_I8
      character(len=1024) :: buffer, io_msg
      integer :: io_stat

      if (command_argument_count() >= 1) then
         call get_command_argument(1, buffer)
         read (buffer, fmt=*, iostat=io_stat, iomsg=io_msg) total_iters
         if (io_stat /= 0) then
            write (unit=error_unit, fmt='(2A)') &
               'Erreur: ', io_msg
            stop 1
         end if
      else
         total_iters = default_total_iters
      end if
   end function get_total_iters
end program main
