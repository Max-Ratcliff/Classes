! example of subroutine defined inside of a module


module sub_03_module

    contains

    subroutine squared(x,x_squared)
        ! compute the square each element of the array x
        implicit none
        real (8), intent(in) :: x(:) ! we don't give an explicit size for x
        real (8), intent(out) :: x_squared(size(x)) ! we set x_squared to be same size as x
        x_squared = x**2
    end subroutine squared

end module sub_03_module

! ---------------------------------------------------

program sub_03
    ! we need to include the module in this program somehow
    implicit none ! always need this
    ! declare arrays here

    ! fill in array values here

    print *, "before calling the subroutine:"
    print *, "    x = ", x
    print *, "    y = ", y
    print *, "--------------------------------------"

    ! call subroutine here

    print *, "after calling the subroutine:"
    print *, "    x = ", x
    print *, "    y = ", y
    print *, "--------------------------------------"

end program sub_03

! instructor notes
! - module must be defined first!
!   - usually modules will be placed in their own source files
! - the size intrinsic function lets you determine array sizes
!   inside of a subroutine, and make it easy to write dimension independent 
!   code (but not rank independent)
! - note we did not use dimension keyword. Pick one syntax and stick to it
! - the declaration of x in the subroutine as x(:) means it has a single index
!   (rank 1) but does not specify how long it is. 
!   - What would we do to declare a rank 2 (indexed by x(i,j)) array?
! - declaring x_squared(size(x)) is more readable and can 
!   allow for compiler optimizations, but is not necessary