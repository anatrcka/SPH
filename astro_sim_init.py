#! /usr/bin/env python
import sys
import os
import matplotlib as mpl
from matplotlib import rc
from pylab import *
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from textwrap import wrap
import math
import numpy as np
import numpy.random as rnd
import scipy
from scipy import interpolate
from scipy.integrate import quad
from scipy.optimize import curve_fit
import collections
plt.rcParams.update({'font.size':16})
namespace = globals()

x = []
y = []
z = []
vx = []
vy = []
vz = []

o=open('sph_init.dat','w')
#o.write("# id, x, y, z, vx, vy, vz, rho, ax, ay, az \n")

print("Initial Conditions")
print("")
print("[0] - Uniformly distributed particles in the box.")
print("[1] - Particles doomed to colide!")
print("[2] - Particles going boom!")
print("[3] - Gravity Falls...")
print("[4] - Just a pipe.")
print("")
choice = eval(input("Select initial conditions: "))
print("")

if choice == 0 :
    # Number of particles N
    N = eval(input("Provide the number of particles: "))

    # initial density
    rho = 0.0

    # initial accelaration
    ax = 0.0
    ay = 0.0
    az = 0.0

    # random initial position of the particles [0,1]
    l = rnd.seed(2)
    x = [rnd.rand() for _ in range(N)]
    m = rnd.seed(4)
    y = [rnd.rand() for _ in range(N)]
    n = rnd.seed(12)
    z = [rnd.rand() for _ in range(N)]

    # random initial velocity of the particles [-1,1]
    p = rnd.seed(28)
    vx = [rnd.uniform(-1, 1) for _ in range(N)]
    q = rnd.seed(56)
    vy = [rnd.uniform(-1, 1) for _ in range(N)]
    r = rnd.seed(66)
    vz = [rnd.uniform(-1, 1) for _ in range(N)]

elif choice == 1 :
    # Number of particles K
    K = eval(input("Provide the number of particles at border [0]: "))
    print("")
    # Number of particles M
    M = eval(input("Provide the number of particles at border [1]: "))
    print("")
    # Total number of particles N
    N = K + M
    print("Total number of particles: "+str(N))
    print("")

    # Sphere radius
    radius = eval(input("Provide the radii for both spheres: "))
    print("")

    # initial density
    rho = 0.0

    # initial accelaration
    ax = 0.0
    ay = 0.0
    az = 0.0

    # random initial position of the K particles
    l = rnd.seed(2)
    phi = [rnd.uniform(0, 2*np.pi) for _ in range(K)]
    m = rnd.seed(4)
    costheta = [rnd.uniform(-1, 1) for _ in range(K)]
    n = rnd.seed(12)
    u = [rnd.uniform(0, 1) for _ in range(K)]

    theta = np.arccos( costheta )

    # Model selection for sphere at border [0]
    print("")
    print("Model selection for sphere at border [0].")
    print("")
    print("[0] - Ghost in the Shell.")
    print("[1] - Thomson's atomic model.")
    print("")
    case = eval(input("Select model case: "))
    print("")

    # Control the sphere position in the box
    shift_x = eval(input("Provide shift on x-axis [0.1]: "))
    shift_y = eval(input("Provide shift on y-axis [0.5]: "))
    shift_z = eval(input("Provide shift on z-axis [0.5]: "))
    print("")

    if case == 0:
        x_k = radius * sin( theta) * cos( phi ) + shift_x
        y_k = radius * sin( theta) * sin( phi ) + shift_y
        z_k = radius * cos( theta ) + shift_z
    elif case == 1:
        x_k = radius * np.cbrt(u) * sin( theta) * cos( phi ) + shift_x
        y_k = radius * np.cbrt(u) * sin( theta) * sin( phi ) + shift_y
        z_k = radius * np.cbrt(u) * cos( theta ) + shift_z

    # --------------------------------------------------------------------------

    # random initial position of the M particles
    l = rnd.seed(1)
    phi = [rnd.uniform(0, 2*np.pi) for _ in range(M)]
    m = rnd.seed(3)
    costheta = [rnd.uniform(-1, 1) for _ in range(M)]
    n = rnd.seed(11)
    u = [rnd.uniform(0, 1) for _ in range(M)]

    theta = np.arccos( costheta )

    # Model selection for sphere at border [1]
    print("")
    print("Model selection for sphere at border [1].")
    print("")
    print("[0] - Ghost in the Shell.")
    print("[1] - Thomson's atomic model.")
    print("")
    case = eval(input("Select model case: "))
    print("")

    # Control the sphere position in the box
    shift_x = eval(input("Provide shift on x-axis [0.9]: "))
    shift_y = eval(input("Provide shift on y-axis [0.5]: "))
    shift_z = eval(input("Provide shift on z-axis [0.5]: "))
    print("")

    if case == 0:
        x_m = radius * sin( theta) * cos( phi ) + shift_x
        y_m = radius * sin( theta) * sin( phi ) + shift_y
        z_m = radius * cos( theta ) + shift_z
    elif case == 1:
        x_m = radius * np.cbrt(u) * sin( theta) * cos( phi ) + shift_x
        y_m = radius * np.cbrt(u) * sin( theta) * sin( phi ) + shift_y
        z_m = radius * np.cbrt(u) * cos( theta ) + shift_z

    # --------------------------------------------------------------------------

    # random initial velocity of the total N particles
    vx_p = eval(input("Provide velocity for sphere at border [0], [10]: "))
    vx_n = eval(input("Provide velocity for sphere at border [1], [-10]: "))

    vx_k = [vx_p for _ in range(K)]
    vx_m = [vx_n for _ in range(M)]
    vy = [0 for _ in range(N)]
    vz = [0 for _ in range(N)]

    # --------------------------------------------------------------------------

    x.extend(x_k)
    x.extend(x_m)
    y.extend(y_k)
    y.extend(y_m)
    z.extend(z_k)
    z.extend(z_m)
    vx.extend(vx_k)
    vx.extend(vx_m)

elif choice == 2 :
    # Number of particles K
    K = eval(input("Provide the number of particles in the inner sphere: "))
    print("")
    # Number of particles M
    M = eval(input("Provide the number of particles in the outer sphere: "))
    print("")
    # Total number of particles N
    N = K + M
    print("Total number of particles: "+str(N))
    print("")

    # initial density
    rho = 0.0

    # initial accelaration
    ax = 0.0
    ay = 0.0
    az = 0.0

    # random initial position of the K particles
    l = rnd.seed(2)
    phi = [rnd.uniform(0, 2*np.pi) for _ in range(K)]
    m = rnd.seed(4)
    costheta = [rnd.uniform(-1, 1) for _ in range(K)]
    n = rnd.seed(12)
    u = [rnd.uniform(0, 1) for _ in range(K)]

    theta = np.arccos( costheta )
    radius_min = eval(input("Provide the radius of the inner sphere [r<0.1]: "))

    # Model selection for inner sphere
    print("")
    print("Model selection for inner sphere.")
    print("")
    print("[0] - Ghost in the Shell.")
    print("[1] - Thomson's atomic model.")
    print("")
    case = eval(input("Select model case: "))
    print("")

    if case == 0:
        x_k = radius_min * sin( theta) * cos( phi ) + 0.5
        y_k = radius_min * sin( theta) * sin( phi ) + 0.5
        z_k = radius_min * cos( theta ) + 0.5
    elif case == 1:
        x_k = radius_min * np.cbrt(u) * sin( theta) * cos( phi ) + 0.5
        y_k = radius_min * np.cbrt(u) * sin( theta) * sin( phi ) + 0.5
        z_k = radius_min * np.cbrt(u) * cos( theta ) + 0.5
    # --------------------------------------------------------------------------

    # random initial position of the M particles
    l = rnd.seed(1)
    phi = [rnd.uniform(0, 2*np.pi) for _ in range(M)]
    m = rnd.seed(3)
    costheta = [rnd.uniform(-1, 1) for _ in range(M)]
    n = rnd.seed(11)
    u = [rnd.uniform(0, 1) for _ in range(M)]

    theta = np.arccos( costheta )
    radius_max = eval(input("Provide the radius of the outer sphere [0.1<r<0.3]: "))

    # Model selection for outer sphere
    print("")
    print("Model selection for outer sphere.")
    print("")
    print("[0] - Ghost in the Shell.")
    print("[1] - Thomson's atomic model.")
    print("")
    case = eval(input("Select model case: "))
    print("")

    if case == 0:
        x_m = radius_max * sin( theta) * cos( phi ) + 0.5
        y_m = radius_max * sin( theta) * sin( phi ) + 0.5
        z_m = radius_max * cos( theta ) + 0.5
    elif case == 1:
        x_m = radius_max * np.cbrt(u) * sin( theta) * cos( phi ) + 0.5
        y_m = radius_max * np.cbrt(u) * sin( theta) * sin( phi ) + 0.5
        z_m = radius_max * np.cbrt(u) * cos( theta ) + 0.5

    # --------------------------------------------------------------------------

    # random initial velocity of the particles [-1,1]
    vx = [0.0 for _ in range(N)]
    vy = [0.0 for _ in range(N)]
    vz = [0.0 for _ in range(N)]

    # --------------------------------------------------------------------------

    x.extend(x_k)
    x.extend(x_m)
    y.extend(y_k)
    y.extend(y_m)
    z.extend(z_k)
    z.extend(z_m)

elif choice == 3 :
    # Number of particles N
    N = eval(input("Provide the number of particles: "))

    # initial density
    rho = 0.0

    # initial accelaration
    ax = 0.0
    ay = 0.0
    az = 0.0

    # random initial position of the particles
    l = rnd.seed(2)
    x = [rnd.rand() for _ in range(N)]
    m = rnd.seed(4)
    y = [rnd.rand() for _ in range(N)]
    n = rnd.seed(12)
    z = [rnd.uniform(0.1, 0.15) for _ in range(N)]

    # random initial velocity of the particles [-1,1]
    p = rnd.seed(28)
    vx = [0. for _ in range(N)]
    q = rnd.seed(56)
    vy = [0. for _ in range(N)]
    r = rnd.seed(66)
    vz = [0. for _ in range(N)]

elif choice == 4 :
    # Number of particles N
    N = eval(input("Provide the number of particles: "))
    print("")

    # Cylinder properties
    radius = eval(input("Provide the radius of the cylinder [0.1<]: "))
    H = eval(input("Provide the length/height of the cylinder [0.5<]: "))
    print("")

    # initial density
    rho = 0.0

    # initial accelaration
    ax = 0.0
    ay = 0.0
    az = 0.0

    # random initial position of the particles
    l = rnd.seed(2)
    theta = [rnd.uniform(0, 2*np.pi) for _ in range(N)]
    m = rnd.seed(4)
    x = [rnd.uniform(0.1, H) for _ in range(N)]
    n = rnd.seed(12)
    u = [rnd.uniform(0, 1) for _ in range(N)]

    # Control the cylinder position in the box
    shift_y = eval(input("Provide shift on y-axis [0.5]: "))
    shift_z = eval(input("Provide shift on z-axis [0.7]: "))
    print("")

    x = x
    y = radius * np.sqrt(u) * np.cos(theta) + shift_y
    z = radius * np.sqrt(u) * np.sin(theta) + shift_z

    # random initial velocity of the particles
    p = rnd.seed(28)
    vx = [1. for _ in range(N)]
    q = rnd.seed(56)
    vy = [0. for _ in range(N)]
    r = rnd.seed(66)
    vz = [0. for _ in range(N)]

# ------------------------------------------------------------------------------

for i in range(0, N):
    o.write('{0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10} \n'.format(i,x[i],y[i],z[i],vx[i],vy[i],vz[i],rho,ax,ay,az))
o.close()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d', axisbg='black')

p=ax.scatter(x, y, z, 'ro',c=vx, cmap='Oranges')

ax.set_xlim(0,1)
ax.set_ylim(0,1)
ax.set_zlim(0,1)

#ax.xaxis.set_ticks(np.arange(3, 9.001, 1))
#ax.yaxis.set_ticks(np.arange(6.5, 12.501, 2))
#ax.zaxis.set_ticks(np.arange(-4, 2.001, 2))

#ax.set_xlabel('')
#ax.set_ylabel('')
#ax.set_zlabel('')
ax.xaxis.labelpad=30
ax.yaxis.labelpad=30
ax.zaxis.labelpad=30
fig.colorbar(p,label='velocity')
plt.show()
