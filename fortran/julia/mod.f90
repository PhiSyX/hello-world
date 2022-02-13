module mod
   use, intrinsic::iso_fortran_env, only: DP => REAL64
   implicit none

   private
   integer, parameter :: MAX_ITERATION = 255
   real(kind=DP), parameter :: MAX_EPSLN = 2.0_DP
   complex(kind=DP), parameter :: C = (-0.622772_DP, 0.42193_DP)

   public :: iterate
contains
   elemental function iterate(i0) result(n)
      implicit none

      complex(kind=DP), &
         intent(in)         :: i0
      integer            :: n
      complex(kind=DP) :: i

      i = i0
      n = 0
      do while (abs(i) < MAX_EPSLN .and. n < MAX_ITERATION)
         i = i**2 + C
         n = n + 1
      end do
   end function iterate
end module mod
