program main
   use::cellular_automata_mod
   implicit none
   integer :: total_cells, rule, max_steps
   type(automaton_t) :: automaton
   integer :: step

   call get_parameters(rule, total_cells, max_steps)

   automaton = initialize_automaton(total_cells, rule)

   call print_rules(automaton)
   call print_automaton(automaton)

   do step = 1, max_steps
      call step_automaton(automaton)
      call print_automaton(automaton)
   end do
contains
   subroutine get_parameters(rule, total_cells, max_steps)
      use, intrinsic::iso_fortran_env, only: error_unit
      implicit none

      integer, intent(out) :: rule, total_cells, max_steps
      integer :: istat
      character(len=1024) :: buffer, io_msg

      rule = 90
      total_cells = 50
      max_steps = 100

      if (command_argument_count() > 0) then
         call get_command_argument(1, buffer)
         read (buffer, fmt=*, iostat=istat, iomsg=io_msg) rule
         if (istat /= 0) then
            print '(2A)', 'Erreur: ', io_msg
            stop 1
         end if
      end if

      if (command_argument_count() > 1) then
         call get_command_argument(2, buffer)
         read (buffer, fmt=*, iostat=istat, iomsg=io_msg) total_cells
         if (istat /= 0) then
            print '(2A)', 'Erreur: ', io_msg
            stop 1
         end if
      end if

      if (command_argument_count() > 2) then
         call get_command_argument(3, buffer)
         read (buffer, fmt=*, iostat=istat, iomsg=io_msg) max_steps
         if (istat /= 0) then
            print '(2A)', 'Erreur: ', io_msg
            stop 1
         end if
      end if
   end subroutine get_parameters
end program main
