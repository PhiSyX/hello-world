FORTRAN_PARAMS = -Wall -Wextra

build:
	@echo "Building..."
	@mkdir -p obj
	gfortran $(FORTRAN_PARAMS) -o obj/hello_world.o \
		./hello_world.f90
	gfortran $(FORTRAN_PARAMS) -o obj/julia_set.o \
		Julia/mod.f90 Julia/set.f90
	gfortran $(FORTRAN_PARAMS) -o obj/julia_openmp_set.o \
		Julia/mod.f90 Julia/openmp_mod.f90 Julia/openmp_set.f90
	@mv *.mod Julia/

clean:
	@echo "Cleaning..."
	rm -f obj/*.o
