module computeError
    use utility, only : fp
    use problemSetup, only : N, dt
    implicit none
    private

    ! Make this public so it can be called from main program
    public :: check_steady_state

    ! local epsilon for the steady state calculation
    real(fp), parameter :: epsilon = 1.0e-6_fp

contains
    ! Checks if diffusion solution has reached steady state
    ! the solution is steady when the L1 norm of the change in the solution is sufficiently small:
    ! \left\|\frac{\Delta u^n}{\Delta t}\right\|_1 \equiv \frac{1}{N \Delta t} \sum_{i=1}^N\left|u_i^n-u_i^{n-1}\right|<\epsilon
    function check_steady_state(u, u_prev) result(is_steady)
        real(fp), dimension(0:N+1), intent(in) :: u, u_prev
        logical :: is_steady
        integer :: i! loop counter 
        real(fp) :: summand, norm_factor ! l1 norm and normalization factor for L1 norm

        ! Initialize variables
        summand = 0.0_fp
        norm_factor = 1.0_fp / (real(N, fp) * dt)

        do i = 1, N
            summand = summand + abs(u(i) - u_prev(i))
        end do

        is_steady = ((summand * norm_factor) < epsilon) 

    end function check_steady_state

end module computeError