! File: dft.f90
! Purpose: Provides discrete Fourier transformation matrices

module dft

  use utility, only : fp, pi
  implicit none

contains

  !!! === 1. function: matvecprod
  function matvecprod(A,x) result(y)
    implicit none
    real(fp), dimension(:,:), intent(in) :: A
    ! in order to be valid multiplication if A is m x n then x must be of size n and y must be of size m 
    real(fp), dimension(size(A,2)), intent(in) :: x
    real (fp), dimension(size(A,1)) :: y
    integer :: i, j 

    ! Ax=y
    ! i loops down row my row
    do i = 1, size(A,1)
      ! j loops across the row to sum it and multiply by x at j 
      do j = 1, size(x) 
        y(i)=y(i) + A(i,j) * x(j) 
      end do
    end do
  end function matvecprod

  ! function: dft_TransMat
  ! purpose: Fill transformation matrix for a discrete Fourier transform
  !          on a given domain, and return compatible set of wavenumbers
  subroutine dft_TransMat(x,k,T)
    implicit none
    real (fp), intent(in)     :: x(:)
    real (fp), intent(out)    :: k(size(x))
    real (fp), intent(in out) :: T(size(x),size(x))
    
    ! Local variables
    integer :: N, i
    real (fp) :: om, dx

    ! Set sizes and base wavenumber
    N = size(x)
    dx = x(2) - x(1)
    om = 2*pi/(N*dx)
    
    ! Set wavenumbers
    k(1) = 0.0_fp
    do i=2,N,2
      k(i) = i*om/2
      if (i+1 <= N) then  
        k(i+1) = k(i)
      end if
    end do
    
    !!! === 2. Add your code to fill T here
    T(1,:) = 1.0_fp/N
    do i = 2, N
      if (mod(i,2) == 0) then
        T(i,:) = (2.0_fp/N)*cos(k(i)*x)
      else
        T(i,:) = (2.0_fp/N)*sin(k(i)*x)
      end if
    end do
  end subroutine dft_TransMat

  !!! === 3. subroutine: dft_InvTransMat
  subroutine dft_InvTransMat(x,k,Tinv)
    implicit none
    real (fp), intent(in)     :: x(:)
    real (fp), intent(in)    :: k(size(x))
    real (fp), intent(in out) :: Tinv(size(x),size(x))

    integer :: N, j
    N = size(x)  

    Tinv(:,1) = 1.0_fp
    do j = 2, N
      if (mod(j,2) == 0) then
        Tinv(:,j) = cos(k(j)*x)
      else
        Tinv(:,j) = sin(k(j)*x)
      end if
    end do
  end subroutine dft_InvTransMat


end module dft
