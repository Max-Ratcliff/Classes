program func_modify_args
    implicit none
    integer, parameter :: fp=selected_real_kind(15)
    real (fp) :: x,y

    x = -2.0_fp
    y = 0.0_fp

    print *, "Before calling function:"
    print *, "    x = ", x
    print *, "    y = ", y
    print *, "--------------------------------------"

    y = squared(x)

    print *, "After calling function:"
    print *, "    x = ", x
    print *, "    y = ", y

contains

    ! write a function called squared that squares a number
    function squared(z) result(z_squared)

    implicit none
    real (8), intent(in out) :: z ! what do you think intent(IN) stands for?
    ! tells function to expect the input to be a real called x
    real(fp) :: z_squared

    z_squared = z**2
    ! z = 5.0_fp bad practice will change x in the original program

end function squared


end program func_modify_args

! instructor notes:
! - note how x changes
! - this is bad practice! Should use subroutines for this.
! - make note of the intent statement again
!   - helps catch bugs
!   - makes code more readable
!   - can actually make code run faster as well