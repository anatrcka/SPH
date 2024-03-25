#include <vector>
#include <string>
#include <iostream>

#include "Particle.hpp"
#include "load_particles.hpp"
#include "leapfrog.hpp"

using namespace std;

int N = 10000;
const double L = 1.;
const double h = 0.01;
const double alpha = 4./3.; // Other possible values 1., 5./3., 2.,...

const double dt  = 0.005;
const int num_steps = 70;

#ifdef NAIVE
const string file_stem = "out_naive/snap_";
#else
const string file_stem = "out/snap_";
#endif

#ifdef GRAVITY
const Vec g(0.,0.,274.);
#endif

void banner(){
    cout << "Welcome to the SPH(d) code for Smoothed Particle Hydrodynamics by PhD students"<< endl;
    cout << "(Copyright 2017: Ana, Angelos, Michele, Shivangee. Made with love @ UGent)" << endl;
    cout << "------------------------------------------------------------------------------" <<endl;
    cout << endl;
    cout << "Parameters: " <<endl;
    cout << " N = " << N << endl;
    cout << " h = " << h << endl;
    cout << " dt = " << dt << endl;
    cout << " num_steps = " << num_steps << endl;
#ifdef GRAVITY
    cout << " g = " << g << endl;
#endif
    cout << endl;
}

string ic_file = "sph_init.dat";


int main(int argc, char const *argv[])
{
    // N = atoi(argv[1]);
    banner();
    vector<Particle> particles = create_particles(N, ic_file);
    leapfrog_integrator(particles, dt, num_steps);
    return 0;
}
