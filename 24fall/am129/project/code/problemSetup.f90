module problemSetup
    use utility, only : fp, maxFileLen, maxStrLen, pi
    use read_initFile

    implicit none
    private

    ! ad_vel: constant advection velocity 
    ! k_diff: consntant diffusion coefficient >= 0
    ! cfl: CFL number initially set to be 1.0
    ! dx, dt: spacial step and temporal step respectively
    ! tmax: max simulation time
    real (fp), public :: ad_vel, k_diff, cfl, dx, dt, tmax
    ! N: integer number of grid points
    integer, public :: N
    ! choose smooth or discontinuous initial condition for advection
    logical, public :: smooth_advec_ic
    character(len=maxStrLen), public :: problem_type, advection_scheme

    ! define arrays where:
    ! x is a spacial grid over the domain (0,1)
    ! u is the solution (u_t) where each element is the solution at the corresponding x value
    ! both arrays will be of lenth N+1 
    real (fp), dimension(:), allocatable, public :: u, x

    ! file names
    character(len=maxStrLen), public :: outFile, runName
    character(len=maxStrLen) :: inFile


    ! make subroutines public
    public :: setup_init, set_ics, deallocate


    ! claude 3.5 sonnet suggestion to get rid of -Wcompare-reals warning
    ! prompted with the compliation warning and "is there a specific real comparison operator in fortran"
    real (fp), public :: epsilon = 1.0e-10
    
contains


    subroutine setup_init()
        ! read in the .init file
        implicit none
        integer :: i ! loop counter

        ! Get name of input file
        inFile = 'simulation.init'
        call get_command_argument(1,inFile)
        print *, "Reading from ",inFile
    
        ! Fill in default values
        ad_vel = 0.0_fp
        k_diff = 1.156_fp ! from the material properties
        cfl = 1.0
        tmax = 2.0
        N = 64
        smooth_advec_ic = 1 ! 1 = true, 0 = false
    
        ! Read problem settings from the input file
        ad_vel = read_initFileReal(infile, 'advection_velocity')
        k_diff = read_initFileReal(infile, 'diffusion_coeff')
        cfl = read_initFileReal(infile, 'cfl')
        tmax = read_initFileReal(infile, 'tmax')
        N = read_initFileInt(infile, 'N')
        if (read_initFileInt(infile, 'smooth_advec_ic') == 1) then
            smooth_advec_ic = .true.
        else
            smooth_advec_ic = .false.
        end if
        !smooth_advec_ic = read_initFileInt(infile, 'smooth_advec_ic')
        advection_scheme = read_initFileChar(infile, 'advection_scheme')

        ! Set the spacial step and temporal step
        ! spacial step is evenly distributed over the domain with N grid points
        !∆x = (xb − xa)/N
        ! xb = 1
        ! xa = 0 
        dx = 1.0_fp/N

        ! dt is dependant on the CFL number and the type of problem we are solving
        ! if ad_vel = 0 then the advection term drops out and we are solving the diffusion equation
        !   C = max(|a_p|)* dt/dx, however the max term can be ignored becuase we are using p = 1
        ! if k_diff = 0 then the diffusion term drops out and we are solving the advection equation
        !   C = max(|k_p|)* 2dt/(dx)^2, however the max term can be ignored becuase we are using p = 1
        ! otherwise we are solving the combined equation
        !   dt = C min(dx/|a|, dx^2/2k)
        if (abs(ad_vel) < epsilon) then ! ad_vel is close enough to zero
            ! pure diffusion case
            dt = cfl*(dx**2)/(k_diff*2.0_fp)
            problem_type = 'diffusion'
        else if (abs(k_diff) < epsilon) then ! k_diff is close enough to zero
            ! Advection case
            dt = cfl*dx/abs(ad_vel)
            problem_type = 'advection'
        else
            ! combined case
            dt = cfl*min(dx/abs(ad_vel), dx**2/(2.0_fp*k_diff))
            problem_type = 'combined'
        end if
        
    
        ! Set the name of the run and echo it out
        runName = read_initFileChar(infile, 'run_name')
        print *, 'Running problem: ', runName
    
        ! Set the output file, note that // does string concatenation
        outFile = 'data/' // trim(runName)

        ! set up domain and solution space where the 0th element and the N+1th element are ghost cells
        allocate(x(0:N+1))
        allocate(u(0:N+1))
        
        ! fill in the spacial grid
        ! left gost cell x_0 = x_a - dx/2:
        x(0) = 0.0_fp - dx/2.0_fp
        ! fill in the rest of the grid
        ! xi = xa + (i − 1/2)dx, i = 1, ..., N
        do i = 1, N
            x(i) = 0 + (i - 1.0_fp/2.0_fp)*dx
        end do
        ! right ghost cell x_(N+1) = x_b + dx/2
        x(N+1) = 1.0_fp + dx/2.0_fp

        ! initialize the solution space, will be over written in set_ics
        u = 0.0_fp


    end subroutine setup_init

    subroutine set_ics()        
        implicit none
        integer :: i ! loop counter

        ! set the initial conditions
        
        ! diffusion case
        !   u(x) = 0◦F for 0 ≤ x < 1,
        !   u(x) = 100◦F for x = 1. 
        if (abs(ad_vel) < epsilon) then ! avoids real comparison error
            u(1:N) = 0.0_fp
            u(N) = 100.0_fp
            ! boundary conditions
            u(0) = 0.0_fp
            u(N+1) = 100.0_fp
        ! advection case
        !   Smooth: u(x, 0) = sin(2πx). 
        !   Discontinuous:
        !       u0(x) = −1, 0 ≤ x < 1/3, 2/3 < x ≤ 1,
        !       u0(x) = 1, 1/3 ≤ x ≤ 2/3
        else if (abs(k_diff) < epsilon) then ! avoids real comparison error
            if (smooth_advec_ic) then
                u(1:N) = sin(2.0_fp*pi*x(1:N))
            else
                do i = 1, N
                    ! domain is already bounded by 0 and 1
                    if (x(i) < 1.0_fp/3.0_fp .or. x(i) > 2.0_fp/3.0_fp) then
                        u(i) = -1.0_fp
                    else
                        u(i) = 1.0_fp
                    endif
                enddo
            end if

            ! boundary conditions
            ! u(0) = u(N) 
            ! u(N+1) = u(1) ,
            u(0) = u(N)
            u(N+1) = u(1)

        ! combined case
        else
            ! TODO for EC
        end if 
    end subroutine set_ics

    subroutine deallocate()
        deallocate(x)
        deallocate(u)
    endsubroutine deallocate
end module problemSetup