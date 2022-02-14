module cellular_automata_mod
   use, intrinsic::iso_fortran_env, only: error_unit
   implicit none

   private
   integer, parameter, public::total_neighbouts = 3

   type, public::automaton_t
      integer, dimension(0:2**total_neighbouts - 1) :: rules
      integer, dimension(:), allocatable :: cells
   end type automaton_t

   public::initialize_automaton, step_automaton, print_automaton, print_rules
contains
   subroutine initialize_rule(rules, rule)
      implicit none

      integer, dimension(0:2**total_neighbouts - 1), intent(out) :: rules
      integer, value :: rule
      integer i

      do i = lbound(rules, 1), ubound(rules, 1)
         rules(i) = mod(rule, 2)
         rule = rule/2
      end do
   end subroutine initialize_rule

   function initialize_automaton(total_rules, rule) result(automaton)
      implicit none
      integer, value :: total_rules, rule
      type(automaton_t) :: automaton

      integer :: i, io_stat
      real :: rand

      allocate (automaton%cells(total_rules), stat=io_stat)
      if (io_stat /= 0) then
         write (unit=error_unit, fmt="(A)") &
            "Erreur: ne peut pas allouer les cellules"
         stop 1
      end if

      do i = 1, total_rules
         call random_number(rand)
         if (rand > 0.5) then
            automaton%cells(i) = 1
         else
            automaton%cells(i) = 0
         end if
      end do
      call initialize_rule(automaton%rules, rule)
   end function initialize_automaton

   function apply_rule(automaton, left, mid, right) result(output)
      implicit none
      type(automaton_t), intent(in) :: automaton
      integer, intent(in) :: left, mid, right
      integer :: output
      output = automaton%rules(4*left + 2*mid + right)
   end function apply_rule

   subroutine step_automaton(automaton)
      implicit none
      type(automaton_t), intent(inout) :: automaton
      integer :: i, prev, last, next

      prev = automaton%cells(size(automaton%cells))
      last = automaton%cells(1)

      do i = 1, size(automaton%cells) - 1
         next = automaton%cells(i)
         automaton%cells(i) = apply_rule(automaton, prev, automaton%cells(i), automaton%cells(i + 1))
         prev = next
      end do

      automaton%cells(i) = apply_rule(automaton, prev, automaton%cells(i), last)
   end subroutine step_automaton

   subroutine print_automaton(automaton)
      use, intrinsic :: iso_fortran_env, only: output_unit
      implicit none
      type(automaton_t), intent(in) :: automaton
      integer :: i
      character :: c

      do i = 1, size(automaton%cells)
         if (automaton%cells(i) == 1) then
            c = '#'
         else
            c = ' '
         end if
         write (unit=output_unit, fmt="(A)", advance="no") c
      end do
      print "(A)", ""
   end subroutine print_automaton

   function convert_to_neighbourhood(index) result(neighbours)
      implicit none
      integer, value :: index
      character(len=3) :: neighbours
      integer :: i

      do i = total_neighbouts, 1, -1
         if (mod(index, 2) == 0) then
            neighbours(i:i) = '0'
         else
            neighbours(i:i) = '1'
         end if
         index = index/2
      end do
   end function convert_to_neighbourhood

   subroutine print_rules(automaton)
      implicit none
      type(automaton_t), intent(in) :: automaton
      integer :: i

      do i = lbound(automaton%rules, 1), ubound(automaton%rules, 1)
         print "(A3, x, I1)", convert_to_neighbourhood(i), automaton%rules(i)
      end do
   end subroutine print_rules
end module cellular_automata_mod
