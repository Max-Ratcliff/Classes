! File: timestep.f90
! Author: Ian May
! Purpose: Provide subroutines to march particle state forward in time

module timestep

  use utility, only: fp

  implicit none
  private

  public :: take_step

contains

  subroutine take_step(dt,mass,pos_old,mom_old,pos_new,mom_new)
    implicit none
    real (fp), intent(in) :: dt
    real (fp), intent(in) :: mass(2)
    real (fp), intent(in) :: pos_old(2,2)
    real (fp), intent(in) :: mom_old(2,2)
    real (fp), intent(out) :: pos_new(2,2)
    real (fp), intent(out) :: mom_new(2,2)
    ! All local variables
    real (fp) :: vel(2,2)
    real (fp) :: acc_old(2,2)
    real (fp) :: acc_new(2,2)
    
    !!! Step 2a: Velocity at current state
    vel(:,1) = mom_old(:,1)/mass(1)
    vel(:,2) = mom_old(:,2)/mass(2)

    ! Acceleration at current state
    acc_old = compute_acceleration(mass,pos_old)
    
    !!! Step 2b: Update position
    pos_new = pos_old + dt*vel + (0.5_fp)*(dt**2)*acc_old
    

    !!! Step 2c: Recalculate acceleration and update velocity
    acc_new = compute_acceleration(mass, pos_new)
    vel = vel + (0.5_fp)*dt*(acc_old + acc_new) 


    ! update momentum from new velocity
    mom_new(:,1) = mass(1)*vel(:,1)
    mom_new(:,2) = mass(2)*vel(:,2)
  end subroutine take_step

  function compute_acceleration(mass,pos) result(acc)
    implicit none
    real (fp), intent(in) :: mass(2)     ! Mass of each particle
    real (fp), intent(in) :: pos(2,2)    ! Position of each particle
    real (fp) :: acc(2,2)                ! Output acceleration of each particle
    
    !!! Step 1a: Local variables
    real (fp) :: dist
    real (fp), dimension(2) :: force 
    

    !!! Step 1b: Find the distance between particles
    !distance formula: square roof of dx^2 + dy^2
    dist = sqrt((pos(1, 2)-pos(1, 1))**2 + (pos(2, 2)-pos(2, 1))**2)
    

    ! Calculate force from inverse square law
    ! Notice that there is an extra power of dist in the denominator
    ! This converts (pos(:,2) - pos(:,1)) to a unit vector
    
    force = mass(1)*mass(2)*(pos(:,2) - pos(:,1))/dist**3

    !!! Step 1c: Scale force to get accelerations
    acc(:,1) = force/mass(1)
    acc(:, 2) = -force/mass(2)

    
    
  end function compute_acceleration

end module timestep
