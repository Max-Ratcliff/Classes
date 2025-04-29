program sub_02
    implicit none
    integer, parameter :: fp = selected_real_kind(15)
    integer :: n
    real (fp), dimension(3) :: x, y

    x = (/2.d0, 4.d0, 6.d0/)
    y = 0.0_fp ! sets every element of y to 0
    n = size(x)

    print *, "before calling the subroutine:"
    print *, "    x = ", x
    print *, "    y = ", y
    print *, "--------------------------------------"

    ! call the subroutine here
    call squared(n, x, y)

    print *, "after calling the subroutine:"
    print *, "    x = ", x
    print *, "    y = ", y
    print *, "--------------------------------------"
end program sub_02

! write the subroutine here
! name it squared
subroutine squared(n, x, x_squared)
    implicit none
    integer, intent(in) :: n
    real (8), dimension(n), intent(in) :: x
    real (8), dimension(n), intent(out) :: x_squared


    x_squared = x**2
    
end subroutine squared


! instructor notes
! - subroutines are more flexible
!   - they can have multiple inputs, but more importantly
!   - they can have multiple outpus (or no outputs)
! - we declare what is an input, what is an output, and what is both 
!   using the intent declarations
! - note that the subroutine is not declared external. 
!   In fact, it isn't declared at all prior to its use.
! - make subroutine work on arrays as well
!   - The length of the array is passed as an argument to the subroutine
!     - This is fortran 77 compatible
!   - In modern fortran (90 and above) there is a way to pass
!   - arguments without passing array size. 
!     - this is better
!     - doing so requires that the subroutine is defined in a module
!       or inside of a program using a contains statement