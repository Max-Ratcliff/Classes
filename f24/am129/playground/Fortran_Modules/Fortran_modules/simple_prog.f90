program simple_prog

    !!! import (use) other modules !!!

    ! you should always have implicit none
    implicit none

    ! declare variables
    integer, parameter :: fp = selected_real_kind(15)
    real (fp) :: y = -2.0_fp

    ! Body of program 
    print *, "in main program"

    print *, "y = ", y

    !!! call subroutine from module here !!!
    call hello_world()

end program simple_prog