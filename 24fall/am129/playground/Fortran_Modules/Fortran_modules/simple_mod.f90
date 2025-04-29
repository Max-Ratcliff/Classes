! write the module here
module simple_mod
    implicit none
    
contains
    subroutine hello_world()
        !prints hello world
        implicit none
        print *, "hello world"
    end subroutine hello_world
    
end module simple_mod

! instructor notes:
! - Note that subroutines and functions go after the contains