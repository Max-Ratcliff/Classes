module output
    use utility, only : fp, maxStrLen
    use problemSetup, only : N, outFile, runName, dt
    implicit none
    private

    ! Public interfaces for use by other modules
    public :: write_line, filter_output, reset_file

contains
    subroutine write_line(u, fileName)
        real (fp), dimension(0:N+1), intent(in) :: u
        character(len=*), intent(in) :: fileName
        integer :: ioStatus ! sucess or failure of opening the file to write

        open(unit=11, file=fileName, status='old', position='append', action='write', IOSTAT=ioStatus)

        if (ioStatus /= 0) then
            print *, "ERROR: Unable to open output file."
            stop
        end if
        
        write(11, *) u(1:N)

        close(11)
    end subroutine write_line

    subroutine reset_file(fileName)
        character(len=*), intent(in) :: fileName
        open(unit=11, file=fileName, status='replace', action='write')
        close(11)
    end subroutine

    subroutine filter_output(fileName, tmax)
        ! subroutine is generated largely by AI. claude 3.5 sonnet provided me with all the file io code
        ! I wrote the logic behind selecting the lines myself using the epsilon idea to compare reals that I used earlier
        ! comments explaining ai code are also written by me
        character(len=*), intent(in) :: fileName
        real(fp), intent(in) :: tmax
        character(len=maxStrLen) :: newFileName
        real(fp), dimension(N) :: u_line
        real(fp) :: t ! time determined by t = i * dt
        integer :: io_status, i, num_lines, lines_written! io_status, loop counter
        logical :: file_exists, include_t
        real(fp) :: epsilon
        
        ! Define file names
        newFileName = fileName // "_filtered.dat"
        
        ! Check if original file exists
        inquire(file=fileName, exist=file_exists)
        if (.not. file_exists) then
            print *, "Error: Original file ", trim(fileName), " not found"
            return
        end if
        
        ! Open original file for reading
        open(unit=12, file=fileName, status='old', action='read', iostat=io_status)
        if (io_status /= 0) then
            print *, "Error opening file for reading"
            return
        end if
        
        ! Open new file for writing
        open(unit=13, file=newFileName, status='replace', action='write', iostat=io_status)
        if (io_status /= 0) then
            print *, "Error opening file for writing"
            close(12)
            return
        end if
        
        ! Read and filter
        epsilon = 1.0e-10_fp ! set the tolerance for real comparison
        i = 0
        num_lines = INT(tmax / dt)
        print *, "Reading and filtering lines...", "tmax = ", tmax, " num_lines = ", num_lines, " dt = ", dt
        do
            read(12, *, iostat=io_status) u_line
            if (io_status /= 0) exit  ! End of file or error
            
            t = real(i, kind=fp) * dt
            ! print *, "t/tmax = ", t/tmax
            ! Write if this is one of our desired time points: 0, 0.2, 0.5, 0.8, 1

            ! t = index of current line dt
            ! tmax = # lines * dt
            ! t/tmax = i * dt / lines * dt
            ! t/tmax = i/# lines


            !print *, "line ", i, " at t = ", t, " with t/tmax = ", t/tmax, "i*20", i*20

            if (t/tmax > 0 .and. lines_written == 0) then
                include_t = .true.
            else if (t/tmax > 0.2 .and. lines_written == 1) then 
                include_t = .true.
            else if (t/tmax > 0.5 .and. lines_written == 2) then
                include_t = .true.
            else if (t/tmax > 0.8 .and. lines_written == 3) then
                include_t = .true. 
            else if (t/tmax > 1 .and. lines_written == 4) then
                include_t = .true.
            end if

            if (include_t) then
                write(13, *) u_line
                print *, "Wrote line: ", i , " at t = ", t, " with t/tmax = ", t/tmax
                lines_written = lines_written + 1
            end if
            
            i = i + 1
            include_t = .false.
        end do
        
        close(12)
        close(13)
        
        ! Optionally, replace original file with filtered file
        ! call execute_command_line('mv ' // trim(newFileName) // ' ' // trim(fileName))
        
    end subroutine filter_output

end module output