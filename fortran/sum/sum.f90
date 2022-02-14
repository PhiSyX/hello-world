program main
   use, intrinsic::iso_fortran_env, only: input_unit
   implicit none

   real :: val, total
   integer :: io_stat

   total = 0.0
   io_stat = 0

   do while (io_stat >= 0)
      read (unit=input_unit, fmt=*, iostat=io_stat) val
      if (io_stat == 0) total = total + val
   end do

   print *, total
end program main
