CXXFLAGS:=-std=gnu++17 -Wall -O3 -MMD -MP -ggdb -pthread -fopenmp -Iext/flat_hash_map

PROGRAMS = sort 

all: $(PROGRAMS)

clean:
	rm -f *~ *.o *.d test $(PROGRAMS)

-include *.

sort: sort.o 
	g++ -std=gnu++17 $^ -o $@ -pthread -fopenmp
