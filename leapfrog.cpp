#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "Vector.hpp"
#include "Particle.hpp"
#include "physics.hpp"
#include "neighbours.hpp"
#include "util.hpp"

using namespace std;

void leapfrog_integrator(vector<Particle> &particles, double dt, int num_steps)
{
    cout << "Start Leapfrog" << endl;
    int snap_counter = 1;

    // Initialize density and acceleration
    update_rho_acc(particles);

    write_init_snapshot(particles);

    // Initial step (advance all the particle positions of half time step)
    for (int i=0; i < particles.size(); ++i)
    {
        Particle &p = particles[i];

        Vec r0 = p.pos;
        Vec v0 = p.vel;

        p.pos = r0 + 0.5 * dt * v0 + 1.0/8.0 * dt*dt * p.acc;
        pull_back_in_box(p.pos);
    }

    double t=0;
    for (int step = 0; step < num_steps; ++step)
    {
        t+=dt;
        cout << fixed << setprecision(5) << t << endl;

        update_rho_acc(particles);

        vector<Vec> velocity_half_time_step(particles.size());

        for (int i=0; i < particles.size(); ++i)
        {
            Vec future_vel = particles[i].vel + dt * particles[i].acc;

            velocity_half_time_step[i] = 0.5 * (future_vel + particles[i].vel);
        }

        std::string filename = write_snapshot(snap_counter, particles, velocity_half_time_step);
        std::cout << " Written file " << filename << std::endl;
        ++snap_counter;

        // Leapfrog algorithm for every particle
        for (int i=0; i < particles.size(); ++i)
        {
            Particle &p = particles[i];

            p.vel = p.vel + dt * p.acc;

            p.pos = p.pos + dt * p.vel;

            pull_back_in_box(p.pos);
        }
    }
}
