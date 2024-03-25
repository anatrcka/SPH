#pragma once

#include "Vector.hpp"

extern int N;
extern const double L;
extern const double h;
extern const double alpha;

extern const Vec g;

extern const std::string file_stem;

struct Particle {
    static unsigned long id_counter;
    const unsigned long id;
    Vec pos;
    Vec vel;
    double rho;
    Vec acc;

    Particle(Vec p={}, Vec v={}, double d=0.0, Vec a={}) :
        id{Particle::id_counter++}, pos{p}, vel{v}, rho{d},acc{a}
        {};

    std::string to_row() const;
};

std::ostream& operator<<(std::ostream& out, Particle p);
