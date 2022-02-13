program main
   implicit none

   integer, parameter :: MAX_VALUE = 10
   integer :: i, j

   do i = 1, MAX_VALUE
      do j = 1, MAX_VALUE
         print '(3I10)', i, j, gcd(i, j)
      end do
   end do
contains
   integer function gcd(i, j)
      implicit none
      integer, value :: i, j

      do while (i /= j)
         if (i > j) then
            i = i - j
         else if (j > i) then
            j = j - i
         end if
      end do
      gcd = i
   end function gcd
end program main
