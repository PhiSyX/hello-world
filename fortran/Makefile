MAKE_PARAMS := --no-print-directory
FORTRAN_PARAMS := -Wall -Wextra

JULIA_MOD := julia/mod.f90 julia/openmp_mod.f90
obj/julia/%.o: julia/%.f90
	@mkdir -p $(@D)
	gfortran $(FORTRAN_PARAMS) -o $@ $(JULIA_MOD) $<
	@mv *.mod julia/

1D_MOD := 1D/mod.f90
obj/1D/%.o: 1D/%.f90
	@mkdir -p $(@D)
	gfortran $(FORTRAN_PARAMS) -o $@ $(1D_MOD) $<
	@mv *.mod 1D/

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
	@$(MAKE) $(MAKE_PARAMS) obj/clamp/clamp.o
	@$(MAKE) $(MAKE_PARAMS) obj/factorial/ref.o
	@$(MAKE) $(MAKE_PARAMS) obj/factorial/inref.o
	@$(MAKE) $(MAKE_PARAMS) obj/factorial/value.o
	@$(MAKE) $(MAKE_PARAMS) obj/factorial/recursion.o
	@$(MAKE) $(MAKE_PARAMS) obj/clamp/optional.o
	@$(MAKE) $(MAKE_PARAMS) obj/expo.o
	@$(MAKE) $(MAKE_PARAMS) obj/iteration/do_statement.o
	@$(MAKE) $(MAKE_PARAMS) obj/iteration/messing_up.o
	@$(MAKE) $(MAKE_PARAMS) obj/iteration/zero_step.o
	@$(MAKE) $(MAKE_PARAMS) obj/iteration/double.o
	@$(MAKE) $(MAKE_PARAMS) obj/algo/sierpinski.o
	@$(MAKE) $(MAKE_PARAMS) obj/parser.o
	@$(MAKE) $(MAKE_PARAMS) obj/sum/infinite_loop.o
	@$(MAKE) $(MAKE_PARAMS) obj/sum/sum.o
	@$(MAKE) $(MAKE_PARAMS) obj/type/info.o
	@$(MAKE) $(MAKE_PARAMS) obj/type/conv.o
	@$(MAKE) $(MAKE_PARAMS) obj/type/overflow.o
	@$(MAKE) $(MAKE_PARAMS) obj/1D/cellular_automata.o
	@$(MAKE) $(MAKE_PARAMS) obj/string/assignment.o
	@$(MAKE) $(MAKE_PARAMS) obj/string/palindrome.o
	@$(MAKE) $(MAKE_PARAMS) obj/string/ascii.o
	@$(MAKE) $(MAKE_PARAMS) obj/pi/pi.o
	@$(MAKE) $(MAKE_PARAMS) obj/pi/pi_openmp.o
	@$(MAKE) $(MAKE_PARAMS) obj/algo/kaprekar.o
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

parser:
	./obj/parser.o ./input/parser.txt

clean:
	@echo "Cleaning..."
	@rm -rf obj/
