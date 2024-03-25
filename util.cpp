#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Particle.hpp"
#include "util.hpp"

using namespace std;

void pull_back_in_box(Vec& pos)
{
    while (pos.x()>1.0 || pos.x()<0.0){
        pos = {abs(1.0-abs(pos.x())), pos.y(), pos.z()};
    }
    while (pos.y()>1.0 || pos.y()<0.0){
        pos = {pos.x(), abs(1.0-abs(pos.y())), pos.z()};
    }
    while (pos.z()>1.0 || pos.z()<0.0){
        pos = {pos.x(), pos.y(), abs(1.0-abs(pos.z()))};
    }
}

string write_snapshot(int snap_counter, const vector<Particle> &particles, const vector<Vec> &velocity_half_time_step)
{
    std::stringstream file_number;
    file_number << setfill('0') << setw(3) << snap_counter;
    std::string filename = file_stem + file_number.str();
    // Open the file
    ofstream outfile(filename);

    // Write header
    outfile << "# id, x, y, z, vx, vy, vz, rho, ax, ay, az" << endl;

    // Write data
    for (int i=0; i < particles.size(); ++i)
    {
        outfile << particles[i].id << ", " << particles[i].pos.to_row() << ", " << velocity_half_time_step[i].to_row() << ", " <<
                   particles[i].rho << ", " << particles[i].acc.to_row() << '\n';
    }
    outfile.close();
    return filename;
}


void write_init_snapshot( const vector<Particle> &particles)
{
    std::stringstream file_number;
    file_number << setfill('0') << setw(3) << 0;
    std::string filename = file_stem + file_number.str();
    // Open the file
    ofstream outfile(filename);

    // Write header
    outfile << "# id, x, y, z, vx, vy, vz, rho, ax, ay, az" << endl;

    // Write data
    for (int i=0; i < particles.size(); ++i)
    {
        outfile << particles[i].to_row() << '\n';
    }
    outfile.close();
}
