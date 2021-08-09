CXXFLAGS:=-std=gnu++17 -Wall -O3 -MMD -MP -ggdb -pthread -fopenmp -Iext/flat_hash_map

PROGRAMS = test

all: $(PROGRAMS)

clean:
	rm -f *~ *.o *.d test $(PROGRAMS)

-include *.

test: test.o 
	g++ -std=gnu++17 $^ -o $@ -pthread -fopenmp
