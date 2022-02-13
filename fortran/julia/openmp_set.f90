program main
   use, intrinsic::iso_fortran_env, only: DP => REAL64, error_unit
   use openmp_mod, only: compute
   implicit none

   complex(kind=DP), dimension(:, :), allocatable :: i
   integer, dimension(:, :), allocatable :: n
   integer :: points

   points = get_size()

   call initialize_i_values(i, points)
   call initialize_n_values(n, points)
   call compute(i, n)
   call log_n_values(n)

   deallocate (i)
   deallocate (n)
contains
   function get_size() result(n)
      use, intrinsic::iso_fortran_env, only: error_unit
      implicit none

      integer, parameter    :: DEFAULT_NUMBER = 100
      character(len=1024) :: buffer, io_msg

      integer :: n
      integer :: io_stat

      if (command_argument_count() == 0) then
         n = DEFAULT_NUMBER
      else if (command_argument_count() == 1) then
         call get_command_argument(1, buffer)
         read (buffer, fmt=*, iostat=io_stat, iomsg=io_msg) n
         if (io_stat /= 0) then
            write (unit=error_unit, fmt='(2A)') &
               "Erreur:", io_msg
            stop 2
         end if
      else
         write (unit=error_unit, fmt='(A)') &
            "Erreur:", "Nombre d'arguments incorrect"
         stop 1
      end if
   end function get_size

   subroutine initialize_i_values(i, points)
      implicit none

      complex(kind=DP), dimension(:, :), allocatable, &
         intent(inout) :: i
      integer, intent(in)   :: points

      real(kind=DP), parameter :: MIN_RE = -1.8_DP, &
                                  MAX_RE = 1.8_DP, &
                                  MIN_IM = -1.8_DP, &
                                  MAX_IM = 1.8_DP

      real(kind=DP) :: delta_re, delta_im

      integer :: c, d, status

      allocate (i(points, points), stat=status)

      if (status /= 0) then
         write (unit=error_unit, fmt='(A)') &
            "Erreur:", "Allocation de i impossible"
         stop 2
      end if

      delta_re = (MAX_RE - MIN_RE)/points
      delta_im = (MAX_IM - MIN_IM)/points

      do d = 1, size(i, 2)
         do c = 1, size(i, 1)
            i(c, d) = cmplx(MIN_RE + (c - 1)*delta_re, &
                            MIN_IM + (d - 1)*delta_im, &
                            kind=DP)
         end do
      end do
   end subroutine initialize_i_values

   subroutine initialize_n_values(n, points)
      implicit none

      integer, dimension(:, :), allocatable, &
         intent(inout) :: n
      integer, intent(in)   :: points

      integer :: status

      allocate (n(points, points), stat=status)

      if (status /= 0) then
         write (unit=error_unit, fmt='(A)') &
            "Erreur:", "Allocation de i impossible"
         stop 2
      end if

      n = 0
   end subroutine initialize_n_values

   subroutine log_n_values(n)
      implicit none

      integer, dimension(:, :), intent(in) :: n
      integer :: i
      character(len=80) :: fmt_str

      write (fmt_str, '(A, I0, A)') '(', size(n, 2), 'I4)'

      do i = 1, size(n, 1)
         print fmt_str, n(i, :)
      end do
   end subroutine log_n_values
end program main
