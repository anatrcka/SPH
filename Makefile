CC = g++

# if debug is set, print debug messages
ifeq ($(DEBUG),1)
	CFLAGS = -g -O0 -std=c++17 -DDEBUG
else
	CFLAGS = -O3 -std=c++17 -DNDEBUG
endif

SRC = Particle.cpp load_particles.cpp physics.cpp util.cpp leapfrog.cpp neighbours.cpp main.cpp
TST = tests/run_tests.cpp tests/test_Vec.cpp

all: sph naive_sph gravity poly gravity_poly

sph: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

naive_sph: $(SRC) 
	$(CC) $(CFLAGS) -DNAIVE $^ -o $@

gravity: $(SRC)
	$(CC) $(CFLAGS) -DGRAVITY $^ -o $@

gravity_poly: $(SRC)
	$(CC) $(CFLAGS) -DGRAVITY -DPOLYTROPIC $^ -o $@

poly: $(SRC)
	$(CC) $(CFLAGS) -DPOLYTROPIC $^ -o $@

neighbours: Particle.cpp load_particles.cpp physics.cpp neighbours.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f naive_sph sph neighbours gravity poly gravity_poly test
	rm -f -r *.dSYM

test: $(TST) Vector.hpp
	$(CC) $(CFLAGS) $(TST) -o $@
	./test

.PHONY: clean all test