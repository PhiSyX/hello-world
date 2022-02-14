program main
   implicit none

   integer :: i

   ! le programme crash à 0.
   do i = 1, 5, zero_step(0)
      print *, i
   end do
contains
   function zero_step(limit) result(step)
      implicit none

      integer, intent(in) :: limit
      integer :: step
      integer :: i

      do i = 10, limit, -1
         step = i
      end do
   end function zero_step
end program main
