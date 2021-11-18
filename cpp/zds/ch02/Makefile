CXXFLAGS:=-std=gnu++17

PROGRAMS = ch01

all: $(PROGRAMS)

clean:
	rm -f *~ *.o *.d test $(PROGRAMS)

-include *.

ch01: ch01-functions.o 
	g++ -std=gnu++17 $^ -o $@.out