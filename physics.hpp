#pragma once
#include <vector>
#include "Particle.hpp"

double omega(double u);

double omegap(double u);

double rho_periodic(double h, const Vec &pos_i, const std::vector<Particle> &particles, double box_size = 1.0);

Vec acceleration_periodic(double h, const Particle &p_i, const std::vector<Particle> &particles, double box_size = 1.0);

Vec acceleration( const Vec &delta_pos, double rho_i, double rho_j);
