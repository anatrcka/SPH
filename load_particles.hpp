#pragma once
#include <vector>
#include "Particle.hpp"

std::vector<Particle> create_particles(int n_part, std::string file_name = "sph_init.dat");
