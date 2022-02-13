FORTRAN_PARAMS = -Wall -Wextra

build:
	@echo "Building..."
	gfortran $(FORTRAN_PARAMS) -o obj/hello_world.o hello_world.f90
	gfortran $(FORTRAN_PARAMS) -o obj/julia/set.o julia/mod.f90 julia/set.f90

clean:
	@echo "Cleaning..."
	rm -f obj/*.o

# add_executable(julia_set_omp.o
# julia_set_mod.f90 julia_set_omp_mod.f90
# julia_set_omp.f90)
# target_compile_options(julia_set_omp.o PUBLIC -fopenmp)
# target_link_options(julia_set_omp.o PUBLIC -fopenmp)
