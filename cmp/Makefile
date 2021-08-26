CXXFLAGS:=-std=gnu++17 

PROGRAMS = unique_ptr 

all: $(PROGRAMS)

clean:
	rm -f *~ *.o *.d test $(PROGRAMS)

-include *.

unique_ptr: src/unique_ptr.o 
	g++ -std=gnu++17 $^ -o src/$@.out && ./src/$@.out
