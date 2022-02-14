program config_parser
   use, intrinsic::iso_fortran_env, only: error_unit, DP => REAL64
   implicit none

   character(len=1024) :: filename

   type::ConfigType
      character(len=1024) :: method
      integer :: total_iters
      real(kind=DP) :: precision
   end type ConfigType

   type(ConfigType) :: config

   if (command_argument_count() /= 1) then
      write (unit=error_unit, fmt="(A)") &
         "Erreur: s'attend au nom du fichier de configuration comme argument"
      stop 1
   end if

   call get_command_argument(1, filename)
   call new_config(config)
   call read_config(filename, config)
   call print_config(config)
contains
   subroutine new_config(config)
      implicit none

      type(ConfigType), intent(out) :: config

      config%method = "none"
      config%total_iters = 0
      config%precision = 1.0D-10
   end subroutine new_config

   subroutine read_config(filename, config)
      use, intrinsic::iso_fortran_env, only: error_unit, iostat_end
      implicit none

      character(len=*), intent(in) :: filename
      type(ConfigType), intent(inout) :: config
      integer, parameter :: file_unit = 20
      character(len=1024) :: io_msg, line, name, result
      integer :: io_stat

      open (unit=file_unit, file=filename, access="stream", &
            status="old", form="formatted", action="read", &
            iomsg=io_msg, iostat=io_stat)
      if (io_stat /= 0) then
         write (unit=error_unit, fmt="(2A)") &
            "Erreur: ", io_msg
         stop 2
      end if

      do
         read (unit=file_unit, fmt="(A)", iostat=io_stat) line
         if (io_stat == iostat_end) exit

         line = adjustl(line)
         if (line(1:1) == "#") cycle

         call split_line(line, name, result, io_stat)

         if (io_stat /= 0) then
            write (unit=error_unit, fmt="(2A)") &
               "Avertissement: ligne de configuration invalide ", trim(line)
            cycle
         end if

         if (name == "method") then
            config%method = trim(result)
         else if (name == "total_iters") then
            read (result, fmt=*, iostat=io_stat, iomsg=io_msg) config%total_iters
            if (io_stat /= 0) then
               write (unit=error_unit, fmt="(2A)") &
                  "Erreur I/O: total_iters: ", trim(io_msg)
               stop 4
            end if
         else if (name == "precision") then
            read (result, fmt=*, iostat=io_stat, iomsg=io_msg) config%precision
            if (io_stat /= 0) then
               write (unit=error_unit, fmt="(2A)") &
                  "Erreur I/O: precision: ", trim(io_msg)
               stop 4
            end if
         else
            write (unit=error_unit, fmt="(3A)") &
               "Erreur de configuration: ", trim(name), " n'est pas un paramètre"
            stop 4
         end if
      end do

      close (unit=file_unit)
   end subroutine read_config

   subroutine split_line(line, name, result, io_stat)
      implicit none

      character(len=*), intent(in) :: line
      character(len=*), intent(out) :: name, result
      integer, intent(out) :: io_stat
      integer :: pos

      pos = index(line, "=")
      if (pos == 0) then
         io_stat = 1
      end if

      name = line(:pos - 1)
      result = adjustl(line(pos + 1:))
      io_stat = 0
   end subroutine split_line

   subroutine print_config(config)
      implicit none

      type(ConfigType), intent(in) :: config

      print "('method = ', A)", trim(config%method)
      print "('total_iters = ', I0)", config%total_iters
      print "('precision = ', E26.15)", config%precision
   end subroutine print_config
end program config_parser
