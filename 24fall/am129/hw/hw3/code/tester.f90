program tester
    use utility, only: fp, pi 
    use dft, only: dft_TransMat, dft_InvTransMat
    implicit none
    real(fp), dimension(3,3) ::T
    real(fp), dimension(3) :: x
    real (fp), dimension(3) :: k

    T(:,1) = (/1, 2, 3/)
    T(:,2) = (/4, 5, 6/)
    T(:,3) = (/7, 8, 9/)
    ! k(:) = (/pi,pi,pi/)
    x(:) = (/5,10,20/)

    print *, T(:,1)
    print *, T(:,2)
    print *, T(:,3)
    print *
    
    call dft_TransMat(x, k, T)

    print *, T(:,1)
    print *, T(:,2)
    print *, T(:,3)
    print *

    call dft_InvTransMat(x, k, T)
    
    print *, T(:,1)
    print *, T(:,2)
    print *, T(:,3)
    print *
    
    
end program tester