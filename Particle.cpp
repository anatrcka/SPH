#include "Particle.hpp"

unsigned long Particle::id_counter = 0;


/*!
 * Print all the Particle information in a format ready to be saved on a file
 */
std::string Particle::to_row() const{
    std::stringstream out;
    out << id << ", " << pos.to_row() << ", " << vel.to_row() << ", " << rho<< ", "<< acc.to_row();
    return out.str();
}

std::ostream& operator<<(std::ostream& out, Particle p) {
    return out << "id:  " << p.id  << "\n"
               << "pos: " << p.pos << "\n"
               << "vel: " << p.vel << "\n"
               << "rho: " << p.rho << "\n"

               << "acc: " << p.acc << std::endl;
}
