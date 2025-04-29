program advect_diff
    use utility
    use read_initFile
    use problemSetup
    use finiteDifference
    use computeError
    use output

    implicit none

    ! Declare variables for problem setup and solution array
    logical :: steady_state
    real (fp), dimension(:), allocatable :: u_prev
    real (fp) :: t ! time counter
    integer :: i ! loop counter
    character(len=maxStrLen) :: fileName

    !character(len=64) :: fileName
    real(fp) :: tolerance = 1.0e-10_fp

    character(len=20) :: schemes(3)  ! Increased length to avoid truncation
    schemes = [character(len=20) :: 'upwind', 'centered', 'lax-wendroff']

    ! Initialize the problem
    call setup_init()
    print *, 'Problem initialized with:'
    print *, '  k_diff =', k_diff
    print *, '  ad_vel =', ad_vel
    print *, '  N =', N
    print *, '  dt =', dt
    print *, '  dx =', dx
    if (ad_vel > 0.0_fp) then
        print *, '  advection scheme =', advection_scheme
        print *, '  smooth_advec_ic =', smooth_advec_ic
    end if

    allocate(u_prev(0:N+1))

    ! Set initial conditions
    call set_ics()
    u_prev = u
    steady_state = .false.
    t = 0.0_fp !intialize to 0 for t = 0
    

    ! set output file name
    ! ~/$AM129_PATH/project/code/data/${runName}_${scheme}.dat
    !fileName = "data/" // trim(runName) // "_" // trim(scheme) // ".dat"
    !fileName = trim(fileName) ! remove funkiness


    ! Diffusion loop
    if (problem_type == 'diffusion') then
        fileName = trim(outFile) // ".dat"
        call reset_file(fileName)
        do while (.not. steady_state)
            call solve_diffusion(u)
            steady_state = check_steady_state(u, u_prev)
            call write_line(u, fileName)
            u_prev = u
            t = t + dt
        end do
        ! output currently contains every time step, instructions say to only output the time steps that satisfy
        ! t/tmax = 0, 0.2, 0.5, 0.8, 1. becuase we are using the file to store the output at each time step instead of storing the
        ! time dimension in memory and then writing to the file we now need to prune the .dat file and/or make a new .dat file that only
        ! contains the desired time steps
        print *, "Time to reach steady state: ", t
        ! Approach: iterate over lines of the file where each line will be t=#*dt we can then check if t/tmax 
        ! is one of the desired values and then write that line to a new file.
        ! prompted claude 3.5 sonnet to help by giving it the two comments I wrote above and it came up with a filter_output function
        ! written in output.f90
        call filter_output(fileName, t)
        ! new .dat file should incude only the time steps that satisfy t/tmax = 0, 0.2, 0.5, 0.8, 1
    end if

    ! Advection loop
    if (problem_type == 'advection') then
        if (smooth_advec_ic) then
            outFile = trim(outFile) // "_" // "smooth"
        else
            outFile = trim(outFile) // "_" // "discontinuous"
        end if
        if (advection_scheme == 'upwind') then
            fileName = trim(outFile) // "_" // "upwind" // ".dat"
            call reset_file(fileName)
            do while (t < tmax)
                call solve_advection_upwind(u)
                call write_line(u, fileName)
                u_prev = u
                t = t + dt
                ! Update u_prev with u_next and t for the next iteration
            end do
        else if (advection_scheme == 'centered') then
            fileName = trim(outFile) // "_" // "centered" // ".dat"
            call reset_file(fileName)
            do while (t < tmax)
                call solve_advection_centered(u)
                call write_line(u, fileName)
                u_prev = u
                t = t + dt
                ! Update u_prev with u_next and t for the next iteration
            end do
        else if (advection_scheme == 'lax-wendroff') then
            fileName = trim(outFile) // "_" // "lax-wendroff" // ".dat"
            call reset_file(fileName)
            do while (t < tmax)
                call solve_advection_lax_wendroff(u)
                call write_line(u, fileName)
                u_prev = u
                t = t + dt
                ! Update u_prev with u_next and t for the next iteration
            end do
        end if

    end if

    ! Deallocate memory
    call deallocate()
    deallocate(u_prev)

end program advect_diff