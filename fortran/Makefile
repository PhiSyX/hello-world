FORTRAN_PARAMS = -Wall -Wextra

build:
	@echo "Building..."
	@mkdir -p obj
	gfortran $(FORTRAN_PARAMS) -o obj/hello_world.o \
		./hello_world.f90
	gfortran $(FORTRAN_PARAMS) -o obj/julia_set.o \
		./julia/mod.f90 julia/set.f90
	gfortran $(FORTRAN_PARAMS) -o obj/julia_openmp_set.o \
		./julia/mod.f90 julia/openmp_mod.f90 julia/openmp_set.f90
	@mv *.mod julia/
	gfortran $(FORTRAN_PARAMS) -o obj/gcd.o \
		./gcd.f90

clean:
	@echo "Cleaning..."
	rm -f obj/*.o
