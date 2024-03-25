#pragma once

#include <vector>
#include "Particle.hpp"


void leapfrog_integrator(std::vector<Particle> &particles,
    double dt,
    int num_steps);
