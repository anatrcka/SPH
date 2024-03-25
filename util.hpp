#pragma once
#include <vector>
#include <fstream>
#include "Vector.hpp"
#include "Particle.hpp"

void pull_back_in_box(Vec& pos);

std::string write_snapshot(int snap_counter,
                           const std::vector<Particle> &particles,
                           const std::vector<Vec> &velocity_half_time_step);

void write_init_snapshot( const std::vector<Particle> &particles);
