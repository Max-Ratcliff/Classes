! /codes/builtinfcns.f90

program builtinfcns

  implicit none
  integer, parameter:: dp = selected_real_kind(15)
  real (dp) :: pi, x, y
  real :: pi_singleprecision

  ! compute pi as arc-cosine of -1:
  pi = acos(-1.0_dp)
  ! even though the output of acos(-1.o_dp) is double precision it is stored in a real (single precision) variable effectivly casting it
  pi_singleprecision = acos(-1.0_dp)

  x = cos(pi)
  y = (exp(3.d0 * log(pi)))**(1.d0/3.d0)  ! need 3.d0 or 3.0_dp for full precision!

  print *, "pi = ", pi
  print *, "pi in sp = ", pi_singleprecision
  print *, "x  = ", x
  print *, "y  = ", y

end program builtinfcns
