# SPH
a C++ program to simulate the time evolution of a collisional fluid.


## Creating initial condition

Use astro_sim_init.py to create initial conditions:

    python astro_sim_init.py

the file `sph_init.dat` will be created for now containing a number (to be changed inside the script) of particles randomly distributed in the phase space (with a uniformly distributed position and velocity)

## Compiling

There are 4 make targets:
 * `sph`: the basic SPH code
 * `naive_sph`: the naive SPH code (no optimization)
 * `gravity`: the SPH code with gravity
 * `poly`: the SPH code with a polytropic equation of state
 * `gravity_poly`: the SPH code with gravity and a polytropic equation of state

To compile all targets run `make`, otherwise run `make <target>`.

To generate executables for debugging run `make DEBUG=1` or `make DEBUG=1 <target>`.

## Some results

![Colliding spheres](collides.gif "Colliding spheres")