program func_01
    implicit none
    integer, parameter :: fp=selected_real_kind(15)
    ! is there a difference between declaring a real number as 
    ! real (kind=8) vs real (8) vs real (fp) vs real (kind=fp)?
    ! 
    ! All Equivilant
    real (fp) :: x, y
    real (fp), external :: my_func ! what do you think external stands for?
    ! tells compiler that the function exists and should output a real even though its not yet defined

    x = -2.d0 ! what do you think the .d0 means here?
    ! sets the precision to double precison
    ! x = -2.0_fp ! is it different than this?
    ! both lines to the exact same thing
    y = squared(x)
    print *, "x =          ", x
    print *, "my_func(x) = ", y

contains    

!real (kind=8) function squared(x)
! sets the output to be a double precision real number 
    function squared(z) result(z_squared)

    implicit none
    real (8), intent(IN) :: z ! what do you think intent(IN) stands for?
    ! tells function to expect the input to be a real called x
    real(fp) :: z_squared

    z_squared = z**2

end function squared


end program func_01
! instructor notes:
! - how many values are returned from this function?
! - how did we specify the return value?
! - modify program so that function is declared inside of program
!   - this is better.
!   - function defined after it is used
!   - can now use fp to define precision
! - specify return value explicitly
!   - this is even better. Even even better, put it in it's own module.
!   - the return type has moved into function body
!   - can give function nice clear name, 
!     but now have simpler output variable inside function