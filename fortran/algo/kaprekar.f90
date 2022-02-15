program main
   use, intrinsic::iso_fortran_env, only: error_unit
   implicit none

   integer, parameter :: total_digits = 4
   integer, dimension(:), allocatable :: digits
   integer :: number, prev, small, large, io_stat

   number = get_argument()
   allocate (digits(total_digits), stat=io_stat)

   if (io_stat /= 0) then
      write (error_unit, '(A)') "Erreur: impossible d'allouer le tableau"
      stop 5
   end if

   prev = 0
   do while (number /= prev)
      call number2digits(number, digits)
      call sort_digits(digits)
      small = digits_to_number(digits)
      large = digits_to_number_rev(digits)
      prev = number
      number = large - small
   end do

   print '(I0)', number
contains
   integer function get_argument() result(number)
      use, intrinsic :: iso_fortran_env, only: error_unit
      implicit none

      character(len=80) :: buffer
      integer :: io_stat, first, n, i
      logical :: ok

      if (command_argument_count() /= 1) then
         write (error_unit, '(A)') &
            'Erreur: un seul argument est attendu'
         stop 1
      end if

      call get_command_argument(1, buffer)
      read (buffer, '(I10)', iostat=io_stat) number

      if (io_stat /= 0) then
         write (error_unit, '(A)') &
            "Erreur: l'argument n'est pas un entier"
         stop 2
      end if

      if (number < 1 .or. number > 9999) then
         write (error_unit, '(A)') &
            "Erreur: l'argument n'est pas compris entre 1 et 9999"
         stop 3
      end if

      ok = .false.
      first = mod(number, 10)
      n = number/10

      do i = 2, 4
         if (first /= mod(n, 10)) then
            ok = .true.
            exit
         end if
         n = n/10
      end do

      if (.not. ok) then
         write (error_unit, '(A)') &
            "Erreur: le nombre doit comporter au moins 2 chiffres distincts"
         stop 4
      end if
   end function get_argument

   subroutine number2digits(number, digits)
      implicit none

      integer, value :: number
      integer, dimension(:), intent(inout) :: digits
      integer :: i

      do i = 1, size(digits)
         digits(i) = mod(number, 10)
         number = number/10
      end do
   end subroutine number2digits

   integer function digits_to_number(digits) result(number)
      implicit none

      integer, dimension(:), intent(in) :: digits
      integer :: factor, i

      number = 0
      factor = 1

      do i = 1, size(digits)
         number = number + factor*digits(i)
         factor = factor*10
      end do
   end function digits_to_number

   integer function digits_to_number_rev(digits) result(number)
      implicit none

      integer, dimension(:), intent(in) :: digits
      integer :: factor, i

      number = 0
      factor = 1

      do i = size(digits), 1, -1
         number = number + factor*digits(i)
         factor = factor*10
      end do
   end function digits_to_number_rev

   subroutine sort_digits(digits)
      implicit none

      integer, dimension(:), intent(inout) :: digits
      integer :: i, j, index, tmp

      do i = 1, size(digits)
         index = i
         do j = i + 1, size(digits)
            if (digits(index) < digits(j)) index = j
         end do

         tmp = digits(index)
         digits(index) = digits(i)
         digits(i) = tmp
      end do
   end subroutine sort_digits
end program main
