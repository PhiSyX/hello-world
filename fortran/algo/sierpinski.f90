program main
   use, intrinsic::iso_fortran_env, only: DP => REAL64, error_unit

   implicit none

   integer, parameter :: NR_POINTS = 3
   integer :: total_iters = 100000

   real(kind=DP), dimension(NR_POINTS, 2) :: edge_points
   real(kind=DP), dimension(2) :: point

   integer :: iter, nr_edge
   total_iters = get_total_iters()

   call initialize_random_seed()
   call initialize_edge_points(edge_points)
   call initialize_point(point)

   do iter = 1, total_iters
      nr_edge = get_edge(NR_POINTS)
      call move(point, edge_points, nr_edge)
      print *, point
   end do
contains
   function get_total_iters() result(total_iters)
      use, intrinsic::iso_fortran_env, only: error_unit
      implicit none

      integer :: total_iters
      character(len=1024) :: buffer

      if (command_argument_count() < 1) then
         write (unit=error_unit, fmt='(A)') &
            "Erreur: un nombre d'itérations doit être spécifié."
         stop 1
      end if

      call get_command_argument(1, buffer)
      read (buffer, fmt=*) total_iters
   end function get_total_iters

   subroutine initialize_random_seed()
      implicit none

      integer :: i, n, clock
      integer, dimension(:), allocatable :: seed

      call random_seed(size=n)
      allocate (seed(n))

      call system_clock(count=clock)
      seed = clock + 37*[(i - 1, i=1, n)]

      call random_seed(put=seed)

      deallocate (seed)
   end subroutine initialize_random_seed

   subroutine initialize_edge_points(points)
      implicit none

      real(kind=DP), dimension(:, :), intent(out) :: points

      points(1, 1) = 0.0_DP
      points(1, 2) = 0.0_DP
      points(2, 1) = 0.5_DP
      points(2, 2) = 1.0_DP
      points(3, 1) = 1.0_DP
      points(3, 2) = 0.0_DP
   end subroutine initialize_edge_points

   subroutine initialize_point(point)
      implicit none

      real(kind=DP), dimension(2), intent(out) :: point

      call random_number(point(1))
      call random_number(point(2))
   end subroutine initialize_point

   function get_edge(nr_edges) result(edge)
      implicit none

      integer, value :: nr_edges
      integer :: edge

      real(kind=DP) :: r

      call random_number(r)
      edge = 1 + int(nr_edges*r)
   end function get_edge

   subroutine move(point, edge_points, nr_edge)
      implicit none

      real(kind=DP), dimension(2), intent(inout) :: point
      real(kind=DP), dimension(:, :), intent(in) :: edge_points

      integer, value :: nr_edge

      point(1) = point(1) + (edge_points(nr_edge, 1) - point(1))/2.0_DP
      point(2) = point(2) + (edge_points(nr_edge, 2) - point(2))/2.0_DP
   end subroutine move
end program main
