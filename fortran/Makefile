MAKE_PARAMS := --no-print-directory

FORTRAN_PARAMS := -Wall -Wextra
JULIA_MOD := julia/mod.f90 julia/openmp_mod.f90

obj/julia/%.o: julia/%.f90
	@mkdir -p $(@D)
	gfortran $(FORTRAN_PARAMS) -o $@ $(JULIA_MOD) $<
	@mv *.mod julia/

obj/%.o: %.f90
	@mkdir -p $(@D)
	gfortran $(FORTRAN_PARAMS) -o $@ $<

build:
	@echo "Building..."
	@$(MAKE) $(MAKE_PARAMS) obj/hello_world.o
	@$(MAKE) $(MAKE_PARAMS) obj/julia/set.o
	@$(MAKE) $(MAKE_PARAMS) obj/julia/openmp_set.o
	@$(MAKE) $(MAKE_PARAMS) obj/gcd.o
	@$(MAKE) $(MAKE_PARAMS) obj/leap_year.o
	@$(MAKE) $(MAKE_PARAMS) obj/calc.o
	@$(MAKE) $(MAKE_PARAMS) obj/distance.o
	@$(MAKE) $(MAKE_PARAMS) obj/clamp.o
	@$(MAKE) $(MAKE_PARAMS) obj/factorial/ref.o
	@$(MAKE) $(MAKE_PARAMS) obj/factorial/inref.o
	@$(MAKE) $(MAKE_PARAMS) obj/factorial/value.o
	@echo "Finished."

leap_year:
	./obj/leap_year.o 2020
	./obj/leap_year.o 2021
	./obj/leap_year.o 2024
	./obj/leap_year.o 1700

calc:
	./obj/calc.o "0.1 + 0.2"
	./obj/calc.o "3.8 - 1.2"
	./obj/calc.o "2.1 * 7.0"

clean:
	@echo "Cleaning..."
	@rm -rf obj/
