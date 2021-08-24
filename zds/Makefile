CXXFLAGS:=-std=gnu++17

PROGRAMS = ch01 ch02 ch03 ch04

all: $(PROGRAMS)

clean:
	rm -f *~ *.o *.d test $(PROGRAMS)

-include *.

ch01: ch01.o 
	g++ -std=gnu++17 $^ -o $@.out

ch02: ch02.o 
	g++ -std=gnu++17 $^ -o $@.out

ch03: ch03.o 
	g++ -std=gnu++17 $^ -o $@.out

ch04: ch04.o 
	g++ -std=gnu++17 $^ -o $@.out
