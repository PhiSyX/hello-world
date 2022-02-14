program main
   use, intrinsic::iso_fortran_env, only: input_unit
   implicit none

   real :: val, total
   integer :: io_stat

   total = 0.0

   do ! infinite loop !
      read (unit=input_unit, fmt=*, iostat=io_stat) val

      if (io_stat < 0) exit
      if (io_stat > 0) cycle

      total = total + val
   end do

   print *, total
end program main
