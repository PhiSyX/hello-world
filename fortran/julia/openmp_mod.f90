module openmp_mod
   use, intrinsic::iso_fortran_env, only: dp => REAL64
   use mod, only: iterate
   implicit none

   public::compute
contains
   subroutine compute(i, n)
      implicit none

      complex(kind=dp), dimension(:, :), intent(in)    :: i
      integer, dimension(:, :), intent(inout) :: n
      integer :: c, d

      !$omp parallel do default(none) private(c) shared(i, n)
      do d = 1, size(i, 2)
         do c = 1, size(i, 1)
            n(c, d) = iterate(i(c, d))
         end do
      end do
      !$omp end parallel do
   end subroutine compute
end module openmp_mod
