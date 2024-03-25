#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include "Vector.hpp"
#include "Particle.hpp"

using namespace std;

int number_of_lines(string file_name){
    int n_lines = 0;
    std::string line;
    std::ifstream myfile(file_name);

    while (std::getline(myfile, line))
    {
        if (line[0] != '#')
            ++n_lines;
    }

    return n_lines;
}


std::vector<Particle> create_particles(int n_part, std::string file_name)
{
    std::vector<Particle> vector_particles(n_part);
    int id;
    double x0, y0, z0, vx0, vy0, vz0, density, ax0, ay0, az0;

    int n_line = number_of_lines(file_name);
    if (n_line < n_part)
    {
        cerr << "You are requiring more particles than those on the Initial Condition file!" << endl;
        cout << file_name << " has "<< n_line << " particles." << endl;
        cerr << "Aborting" << endl;
        exit(2);
    }

    ifstream init;
    init.open(file_name);

    for (int i = 0; i < n_part; ++i)
    {
        Particle& p = vector_particles[i];

        init >> id >> x0 >> y0 >> z0 >> vx0 >> vy0 >> vz0 >> density >> ax0 >> ay0 >> az0;

        p.pos = { x0, y0, z0 };
        p.vel = { vx0, vy0, vz0 };
        p.acc = { ax0, ay0, az0 };
        p.rho = density;
    }
    return vector_particles;
}
