CXXFLAGS:=-std=gnu++17

PROGRAMS = ch01 ch02 ch03 ch04 ch05 ch06

all: $(PROGRAMS)

clean:
	rm -f *~ *.o *.d test $(PROGRAMS)

-include *.

ch01: ch01-iostream-println.o 
	g++ -std=gnu++17 $^ -o $@.out

ch02: ch02-memory.o 
	g++ -std=gnu++17 $^ -o $@.out

ch03: ch03-condition.o 
	g++ -std=gnu++17 $^ -o $@.out

ch04: ch04-loop.o 
	g++ -std=gnu++17 $^ -o $@.out

ch05: ch05-array.o 
	g++ -std=gnu++17 $^ -o $@.out

ch06: ch06-stream.o
	g++ -std=gnu++17 $^ -o $@.out