#include <cmath>
#include <math.h>
#include <vector>
#include "Particle.hpp"
#include "Vector.hpp"
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif
using namespace std;

double omega(double u) {
    return 8./M_PI*
            ((u<0.5 && u>=0.) ? (1.-6.*u*u*(1.-u)) :
            (u<1. && u>=0.5) ? (2.*(1.-u)*(1.-u)*(1.-u)) :
            (u>=1.) ? 0. : -INFINITY);
}


double omegap(double u) {
    return -48./M_PI *
            ((u<0.5 && u>=0.) ? u*(2.-3.*u) :
            (u<1. && u>=0.5) ? (1.-u)*(1.-u) :
            (u>=1.) ? 0. : -INFINITY);
}

Vec compute_shift(double h, const Vec &my_pos, const Vec &other_pos, double L = 1.0) {
    double shift_x = 0, shift_y = 0, shift_z = 0;
    if (my_pos.x() < h && (L - other_pos.x()) < h)
        shift_x = L;
    if ((L - my_pos.x()) < h && other_pos.x() < h)
        shift_x = -L;

    if (my_pos.y() < h && (L - other_pos.y()) < h)
        shift_y = L;
    if ((L - my_pos.y()) < h && other_pos.y() < h)
        shift_y = -L;

    if (my_pos.z() < h && (L - other_pos.z()) < h)
        shift_z = L;
    if ((L - my_pos.z()) < h && other_pos.z() < h)
        shift_z = -L;

    return Vec(shift_x, shift_y, shift_z);
}

double rho_periodic(double h, const Vec &pos_i, const vector<Particle> &particles, double box_size) {
    double sum=0;
    for (const Particle &p_j : particles) {
        const Vec shift = compute_shift(h, pos_i, p_j.pos, box_size);
        double o = omega((pos_i - p_j.pos + shift).norm()/h);
        sum += o;
    }
    return sum/particles.size()/(h*h*h);
}


Vec acceleration_periodic(double h, const Particle &p_i, const vector<Particle> &particles, double box_size) {
    Vec sum;
    for (const Particle &p_j : particles) {
        if (p_i.id != p_j.id) {

            const Vec shift = compute_shift(h, p_i.pos, p_j.pos, box_size);
            Vec dist = p_j.pos - p_i.pos - shift;
            Vec a = omegap(dist.norm() / h) * (dist / dist.norm()) * (1 / p_i.rho + 1 / p_j.rho);
            sum += a;
        }
    }
    return sum/particles.size()/(h*h*h*h);
}



Vec acceleration( const Vec &delta_pos, double rho_i, double rho_j) {
    #ifdef POLYTROPIC
    rho_i = pow(rho_i,2.-alpha);
    rho_j = pow(rho_j, 2. - alpha);
    #endif
    Vec a = -omegap(delta_pos.norm() / h) * (delta_pos / delta_pos.norm()) * (1. / rho_i + 1. / rho_j);
    return a/N/(h*h*h*h);
}

//#endif



//Vec gravity(const Vec ){

//}
