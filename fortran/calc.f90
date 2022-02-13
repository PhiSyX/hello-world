program main
   use, intrinsic::iso_fortran_env, only: DP => REAL64
   implicit none

   real(kind=DP) :: x, y
   character(len=1) :: operation

   call get_expression(operation, x, y)
   print "(E15.7)", evaluate(operation, x, y)
contains
   subroutine get_expression(operation, x, y)
      use, intrinsic::iso_fortran_env, only: error_unit
      implicit none

      character, intent(out) :: operation
      real(kind=DP), intent(out) :: x, y

      character(len=1024) :: buffer
      character(len=1024) :: io_msg
      integer :: io_stat

      if (command_argument_count() /= 1) then
         write (unit=error_unit, fmt="(A)") &
            "Erreur: nombre d'arguments incorrect"
         stop 1
      end if

      call get_command_argument(1, buffer)

      print *, trim(buffer)

      read (buffer, fmt=*, iostat=io_stat, iomsg=io_msg) x, operation, y
      if (io_stat /= 0) then
         write (unit=error_unit, fmt="(2A)") &
            "Erreur d'analyse: ", io_msg
         stop 2
      end if
   end subroutine get_expression

   real(kind=DP) function evaluate(operation, x, y)
      use, intrinsic::iso_fortran_env, only: error_unit
      implicit none

      character(len=1), intent(in) :: operation
      real(kind=DP), intent(in) :: x, y

      select case (operation)
      case ("+")
         evaluate = x + y
      case ("-")
         evaluate = x - y
      case ("*")
         evaluate = x*y
      case default
         write (unit=error_unit, fmt="(3A)") &
            "Erreur: ", operation, " n'est pas un opérateur valide"
         stop 3
      end select
   end function evaluate
end program main
