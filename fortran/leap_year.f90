program main
   implicit none

   integer :: year
   year = get_year()

   if (is_leap_year(year)) then
      print "(I0, A)", year, " est une année bissextile"
   else
      print "(I0, A)", year, " n'est pas une année bissextile"
   end if
contains
   integer function get_year()
      use, intrinsic::iso_fortran_env, only: error_unit
      implicit none
      character(len=256) :: buffer

      if (command_argument_count() /= 1) then
         write (unit=error_unit, fmt="(A)") &
            "Erreur:", "Un seul argument est attendu"
         stop 1
      end if

      call get_command_argument(1, buffer)
      read (buffer, "(I10)") get_year
   end function get_year

   logical function is_leap_year(year)
      implicit none

      integer, intent(in) :: year

      if (mod(year, 4) /= 0) then
         is_leap_year = .false.
      else if (mod(year, 100) == 0) then
         if (mod(year, 400) == 0) then
            is_leap_year = .true.
         else
            is_leap_year = .false.
         end if
      else
         is_leap_year = .true.
      end if
   end function is_leap_year
end program main
