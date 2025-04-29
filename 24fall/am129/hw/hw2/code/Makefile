FC = gfortran
FFLAGS = -Wall -Wextra -ffree-line-length-none -fPIC -g -fcheck=all -fbacktrace
LFLAGS = 

OBJ = utility.o timestep.o orbits.o

orbits.ex: $(OBJ)
	$(FC) -o $@ $^ $(LFLAGS) $(FFLAGS)

%.o: %.f90
	$(FC) -c -o $@ $< $(FFLAGS)

.PHONY: clean

clean:
	rm -f orbits.ex *.o *.mod *~
