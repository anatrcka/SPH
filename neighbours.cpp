#include <iomanip>
#include <iostream>
#include <vector>
#include <tuple>

#include "Vector.hpp"
#include "Particle.hpp"
#include "load_particles.hpp"
#include "physics.hpp"
#include "leapfrog.hpp"

using namespace std;

const int M = floor(L/h);


tuple <int, double> side (int s){
    if (s<0)
        return make_tuple(M-1,-L);

    else if (s>M-1)
        return make_tuple(0,L);
    else
        return make_tuple(s,0.);
}


#ifdef NAIVE
/*!
 * Naive implementation for updating particle densities (rho) and accelerations (acc).
 * Iterates over all particles without considering proximity.
 */
void update_rho_acc(vector<Particle>& particles)
{
    for (auto& p : particles)
        p.rho = rho_periodic(h, p.pos, particles);

    for (auto& p : particles)
        p.acc = acceleration_periodic(h, p, particles);
}
#else
/*
 * Optimized implementation using a linked-cell algorithm to update densities and accelerations. 
 * We divide the spatial domain in identical cubical cells and build a list of the particles located in each cell. 
 * If the cell size is larger than the smoothing length, a particle in any given cell can be influenced only by the 
 * particles in neighboring cells (taking into account periodic boundaries).
 */
void update_rho_acc(std::vector<Particle> &particles)
{

    vector < vector <vector<int>  > > header(M,vector<vector<int> >(M,vector <int>(M,-1)));

    vector <int> link(N,-1);
    // initialising header and link
    for(int i = 0 ; i < N ; ++i){

        int ix = (int) (M*particles[i].pos.x());
        int iy = (int) (M*particles[i].pos.y());
        int iz = (int) (M*particles[i].pos.z());

        link[i]=header[ix][iy][iz];
        header[ix][iy][iz]=i;
    }

    for (Particle &p : particles){
        p.rho = 0.0;
        p.acc = {};
    }

    // looping over every cell
    vector<vector <int >> overall_id;
    vector<vector< Vec >> overall_shift;
    vector< int > overall_locnum;

    for(int i = 0 ; i < M ; ++i){
        for (int j=0; j<M; ++j){
            for (int k=0; k<M; ++k){
                if (header[i][j][k] != -1){
                    vector<int> id;

                    int icnt=0;
                    int locnum;
                    int jj=header[i][j][k];
                    vector <Vec> shift;

                    // finding every particle inside the cell
                    while (jj>-1){

                        id.push_back(jj);
                        jj=link[jj];

                        icnt=icnt+1;
                        locnum=icnt;
                    }

                    // looping over 1/2 of the neighbouring cells
                    for (int m = i; m<= i+1;++m){
                        int mt, nt,pt;
                        double t_x,t_y,t_z;
                        tie(mt,t_x)=side(m);

                        if (m==i){
                            for (int n = j-1; n<= j;++n){
                                tie(nt,t_y)=side(n);

                                if (n==j-1){
                                    for (int p = k-1; p<= k+1;++p){
                                        tie(pt,t_z)=side(p);

                                        int jj=header[mt][nt][pt];

                                        // finding every particle inside the neighbouring cells_1
                                        while (jj>-1){
                                            id.push_back(jj);
                                            shift.push_back({t_x,t_y,t_z});

                                            jj=link[jj];
                                            icnt=icnt+1;
                                        }
                                    }
                                }
                                else {
                                    int p=k-1;
                                    tie(pt,t_z)=side(p);

                                    int jj=header[mt][nt][pt];

                                    // finding every particle inside the neighbouring cells_2
                                    while (jj>-1){
                                        id.push_back(jj);
                                        shift.push_back({t_x,t_y,t_z});

                                        jj=link[jj];
                                        icnt=icnt+1;
                                    }

                                }
                            }
                        }

                        else {
                            for (int n=j-1;n<=j+1;++n){
                                tie(nt,t_y)=side(n);
                                for (int p=k-1;p<=k+1;++p){
                                    tie(pt,t_z)=side(p);

                                    int jj=header[mt][nt][pt];

                                    // finding every particle inside the neighbouring cells_3
                                    while (jj>-1){
                                        id.push_back(jj);
                                        shift.push_back({t_x,t_y,t_z});

                                        jj=link[jj];
                                        icnt=icnt+1;
                                    }
                                }
                            }
                        }

                    }


                    // loops for rho
                    for (int c1 = 0; c1<locnum; ++c1){
                        int id_p=id[c1];
                        Particle the_p=particles[id_p];
                        for (int c2 = c1; c2<id.size(); ++c2){

                            int id_n=id[c2];
                            Particle the_n=particles[id_n];

                            Vec vec= the_p.pos-the_n.pos;
                            if (c2>=locnum) vec-=shift[c2-locnum];


                            double Om=omega(vec.norm()/h)/N/(h*h*h);
                            particles[id_p].rho+=Om;


                            if (id_n!=id_p) particles[id_n].rho+=Om;

                        }
                    }
                overall_shift.push_back(shift);
                overall_id.push_back(id);
                overall_locnum.push_back(locnum);

                }
            }
        }
    }

    // looping over every cell
    int cell_counter=0;
    for(int i = 0 ; i < M ; ++i){
        for (int j = 0; j < M; ++j){
            for (int k = 0; k < M; ++k){
                if (header[i][j][k] != -1){
                    // loops for acc
                    for (int c1 = 0; c1<overall_locnum[cell_counter]; ++c1){
                        int id_p=overall_id[cell_counter][c1];
                        Particle the_p=particles[id_p];
                        for (int c2 = c1+1; c2<overall_id[cell_counter].size(); ++c2){

                            int id_n=overall_id[cell_counter][c2];
                            Particle the_n=particles[id_n];

                            Vec vec= the_p.pos-the_n.pos;
                            if (c2>=overall_locnum[cell_counter])
                                vec-=overall_shift[cell_counter][c2-overall_locnum[cell_counter]];

                            Vec a=acceleration(vec, particles[id_p].rho,particles[id_n].rho );
                            particles[id_p].acc+=a;
                            particles[id_n].acc-=a;
                        }
                    }
                ++cell_counter;
                }
            }
        }
    }
    #ifdef GRAVITY
    for (int i=0; i < particles.size(); ++i)
    particles[i].acc+=g;
    #endif
}

#endif
