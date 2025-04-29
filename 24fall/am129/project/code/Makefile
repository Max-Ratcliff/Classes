FC = gfortran
FFLAGS = -g -Wall -Wextra -Wimplicit-interface -fmax-errors=5

OBJ = utility.o read_initFile.o problemSetup.o finiteDifference.o computeError.o output.o advect_diff.o

diff.ex: $(OBJ)
	$(FC) $(FFLAGS) -o $@ $^

%.o: %.f90
	$(FC) $(FFLAGS) -c $<

.PHONY: clean

clean:
	rm -f diff.ex *.o *.mod
