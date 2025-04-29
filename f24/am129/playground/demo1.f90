! /codes/demo1.f90
! Fortran 90 program illustrating data types.
! Note that you should pick one convention, rather
! than mixing matching like this file does. This is
! just for illustration

program demo1

  implicit none
  integer, parameter :: fp = selected_real_kind(7)
  integer, parameter :: dp = selected_real_kind(15)
  real :: x
  real (kind=dp) :: y,z
  integer :: m
  real (dp) :: i

  ! Comment out implicit none and the declaration for i, then observe
  ! what the following lines do:
  ! i = 10
  ! print*, i
  ! i = 10.2394872938479287
  ! print *,i
  ! a = 1.3234
  ! print*,a

  m = 3
  print *, " "
  print *, "M = ", M   ! note that M is the same as m  (case insensitive)


  print *, " "
  print *, "x is real (kind=4)"
  x = 1.0_fp + 1.23456789e-6_fp
  print *, "x = ", x


  print *, " "
  print *, "y is real (kind=8)"
  print *, "  but 1.e0 is real (kind=4):"
  y = 1.0_fp + 1.23456789e-6_fp
  print *, "y = ", y


  print *, " "
  print *, "z is real (kind=8)"
  print *, "  and 1.d0 is real (kind=8):"
  ! The following line is equivalent to z = 1.d0 + 1.23456789d-6
  z = 1.0_dp + 1.23456789e-6_dp
  print *, "z = ", z

end program demo1
