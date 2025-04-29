program sub_01
    implicit none
    integer, parameter :: fp = selected_real_kind(15)
    real (fp) :: x, y

    x = -2.0_fp
    y = 0.0_fp

    print *, "before calling the subroutine:"
    print *, "    x = ", x
    print *, "    y = ", y
    print *, "--------------------------------------"

    ! call the subroutine here
    call squared(x, y)

    print *, "after calling the subroutine:"
    print *, "    x = ", x
    print *, "    y = ", y
    print *, "--------------------------------------"
end program sub_01

! write the subroutine here
! name it squared
subroutine squared(x, x_squared)
    implicit none
    real (8), intent(in) :: x
    real (8), intent(out) :: x_squared


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