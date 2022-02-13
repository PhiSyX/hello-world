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
	gfortran $(FORTRAN_PARAMS) -o obj/leap_year.o \
		./leap_year.f90

leap_year:
	./obj/leap_year.o 2020
	./obj/leap_year.o 2021
	./obj/leap_year.o 2024
	./obj/leap_year.o 1700

clean:
	@echo "Cleaning..."
	rm -f obj/*.o
