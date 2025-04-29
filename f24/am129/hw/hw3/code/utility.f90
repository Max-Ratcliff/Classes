! File: utility.f90
! Purpose: Define useful constants

module utility
  
  implicit none
  
  integer, parameter :: fp = selected_real_kind(15)
  integer, parameter :: maxFileLen = 50
  integer, parameter :: maxStrLen = 100
  real (fp), parameter :: pi = acos(-1.0_fp)

contains
  
end module utility
