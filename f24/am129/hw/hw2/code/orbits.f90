! File: Orbits.f90
! Author: Ian May

program orbits

  use utility, only: fp
  use timestep, only: take_step

  implicit none
  
  integer, parameter :: nSteps = 1000  ! Number of time steps to use
  integer :: nT                        ! Loop variable for time updates
  !!! Step 1
  real (fp) :: tFinal, dt
  real (fp), dimension(2) :: mass
  real (fp), dimension(2, 2, nSteps) :: pos, mom
  

  tFinal = 50
  dt = tFinal/nSteps

  ! Give all particles initial mass, position, and momentum
  call set_ics()

  ! Fill rest of the array by integrating in time
  do nT=1,nSteps-1
    call take_step(dt,mass,pos(:,:,nT),mom(:,:,nT),pos(:,:,nT + 1),mom(:,:,nT + 1))
  end do

  ! Write the arrays to a file for plotting
  !print *, "(x,y) mommentum of particle 1:", mom(:,1, 1000)
  !print *, "(x,y) mommentum of particle 2:", mom(:,2, 1000)
  !print *, "sum of masses in x", mom(1,2, 1000) + mom(1, 1, 1000)
  !print *, "sum of masses in x", mom(2,2, 1000) + mom(2, 1, 1000)
  call write_data()

contains

  subroutine set_ics()
    implicit none
    !!! Step 2
    ! Set particle masses
    mass = (/1.0_fp, 0.01_fp/) ! default
    !mass = (/1.0_fp, 0.1_fp/) ! scenario one
    !mass = (/1.0_fp, 1.0_fp/) ! scenario 3 

    
    ! First particle position and momentum
    pos(:, 1, 1) = (/0.0_fp, 0.0_fp/)
    mom(:, 1, 1) = mass(1) * (/0.0_fp, 0.0_fp/) !default
    !mom(:, 1, 1) = mass(1) * (/-1.0_fp, 0.0_fp/) !scenario 2
    !mom(:, 1, 1) = mass(1) * (/0.0_fp, 1.0_fp/) !scenario 3
    
    ! Second particle position and momentum
    pos(:, 2, 1) = (/0.0_fp, -1.0_fp/)
    mom(:, 2, 1) = mass(2) * (/1.0_fp, 0.0_fp/)


    
  end subroutine set_ics

  subroutine write_data()
    implicit none
    open(20,file = "sol.dat",status = "replace")
    do nT=1,nSteps
      write(20,*) pos(:,:,nT),mom(:,:,nT)
    end do
    close(20)
  end subroutine write_data

end program orbits
