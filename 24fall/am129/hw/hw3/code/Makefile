FC = gfortran
FFLAGS = -g -Wall -Wextra -Wimplicit-interface -fmax-errors=5

OBJ = utility.o problemsetup.o dft.o bvp.o
#OBJ = utility.o dft.o tester.o

bvp.ex: $(OBJ)
	$(FC) $(FFLAGS) -o $@ $^

%.o: %.f90
	$(FC) $(FFLAGS) -c $<

.PHONY: clean

clean:
	rm -f bvp.ex *.o *.mod
