module finiteDifference
    use utility, only : fp, pi
    use problemSetup, only: ad_vel, k_diff, cfl, dx, dt, tmax, N, x, smooth_advec_ic
    
    implicit none
    private
    
    ! Make the key subroutines public so they can be called from the main program
    public :: solve_diffusion, solve_advection_upwind, solve_advection_centered, solve_advection_lax_wendroff


    ! doc strings and headers are partially AI generated with Tweaks for accuracy and helpfullness from me
    ! no other code uses llm help
contains
    ! computes the next time step of the diffusion equation
    ! finite difference scheme from equation (1.22): u_i = u_i + κ∆t/∆x2(un_i+1 − 2u_i + u_i−1)
    subroutine solve_diffusion(u)
        real (fp), dimension(0:N+1), intent(inout) :: u
        ! k*(dt/dx^2) is a constant cause its terms are constant 
        real (fp) :: const_term

        const_term = k_diff * dt / dx**2
        
        u(1:N) = u(1:N) + const_term * (u(2:N+1) - 2.0_fp * u(1:N) + u(0:N-1))

        u(0) = 0.0_fp
        u(N+1) = 0.0_fp

    end subroutine solve_diffusion

    ! computes the next time step for the first order accurate upwind scheme 
    ! 1st order accurate upwind scheme from equation (1.30): u_i = u_i − a∆t/∆x(u_i − u_i−1)
    subroutine solve_advection_upwind(u)
        real (fp), dimension(0:N+1), intent(inout) :: u
        ! a*(dt/dx) is a constant cause its terms are constant 
        real (fp) :: const_term

        const_term = ad_vel * dt / dx

        u(1:N) = u(1:N) - const_term * (u(1:N) - u(0:N-1))


        u(0) = u(N)
        u(N+1) = u(1)

    end subroutine solve_advection_upwind

    ! computes the next time step for the 2nd order accurate centered scheme 
    ! 2nd order accurate centered scheme from equation (1.32): u_i = u_i − a∆t/2∆x(u_i+1 − u_i−1)
    subroutine solve_advection_centered(u)
        real(fp), intent(inout) :: u(0:N+1)
        ! a*(dt/2dx) is a constant cause its terms are constant 
        real (fp) :: const_term

        const_term = (ad_vel * dt)/(2.0_fp * dx)

        u(1:N) = u(1:N) - const_term * (u(2:N+1) - u(0:N-1))

        u(0) = u(N)
        u(N+1) = u(1)

    end subroutine solve_advection_centered

    ! computes the next time step for
    ! 2nd order accurate Lax-Wendroff scheme from equation (1.35): u_i = u_i − a∆t/2∆x(u_i+1 − u_i−1) + 1/2(a∆t/∆x)^2 (u_i+1 − 2u_i + u_i−1)
    subroutine solve_advection_lax_wendroff(u)
        real(fp), intent(inout) :: u(0:N+1)
        ! a*(dt/2dx) is a constant cause its terms are constant 
        ! 1/2(a∆t/∆x)^2 is a constant cause its terms are constant 
        real (fp) :: const_term_A, const_term_B

        const_term_A = (ad_vel * dt)/(2.0_fp * dx)
        const_term_B = 0.5_fp * (ad_vel * dt / dx)**2

        u(1:N) = u(1:N) - const_term_A * (u(2:N+1) - u(0:N-1)) + const_term_B * (u(2:N+1) - 2.0_fp * u(1:N) + u(0:N-1))

        u(0) = u(N)
        u(N+1) = u(1)
    end subroutine solve_advection_lax_wendroff

    !subroutine solve_advection_diffusion(u)
        !real(fp), intent(inout) :: u(0:N+1)
        ! Purpose: Implements the Strang splitting method for the combined
        !          advection-diffusion equation (for extra credit)
    !end subroutine solve_advection_diffusion

end module finiteDifference