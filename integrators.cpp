////////////////////////////////////////////////////////////////////////////////
// ---------------- Particle(s) in Electromagnetic Fields ------------------- //
////////////////////////////////////////////////////////////////////////////////
// This file contains the equations of motion and numerical integrators.      //
////////////////////////////////////////////////////////////////////////////////

#include "particle_em.h"

// Right-hand side of the equation of motion
void dYdt(double t, double *Y, double *R)
{
    int Npart;

#if CASE == CASE_X_POINT
    Npart = NPART_XPOINT;
#else
    Npart = 1;
#endif

    for (int p = 0; p < Npart; p++) {

        int i = 6 * p;

        double x  = Y[i + 0];
        double y  = Y[i + 1];
        double z  = Y[i + 2];

        double vx = Y[i + 3];
        double vy = Y[i + 4];
        double vz = Y[i + 5];

        double Ex, Ey, Ez;
        double Bx, By, Bz;

        ElectricField(x, y, z, t, Ex, Ey, Ez);
        MagneticField(x, y, z, t, Bx, By, Bz);

        // Lorentz acceleration:
        // dv/dt = (q/m) * [ E + (v/c) x B ]
        double ax = QOM * (Ex + (vy * Bz - vz * By) / C_LIGHT);
        double ay = QOM * (Ey + (vz * Bx - vx * Bz) / C_LIGHT);
        double az = QOM * (Ez + (vx * By - vy * Bx) / C_LIGHT);

        R[i + 0] = vx;
        R[i + 1] = vy;
        R[i + 2] = vz;

        R[i + 3] = ax;
        R[i + 4] = ay;
        R[i + 5] = az;
    }
}


// Runge-Kutta 4th-order method
void RK4(double t, double *Y, void (*Ydot)(double, double *, double *), double dt, int neq)
{
    int i;
    double k1[NMAX_EQ], k2[NMAX_EQ], k3[NMAX_EQ], k4[NMAX_EQ];
    double Y1[NMAX_EQ];

    Ydot(t, Y, k1);
    for (i = 0; i < neq; i++) Y1[i] = Y[i] + 0.5 * dt * k1[i];

    Ydot(t + 0.5 * dt, Y1, k2);
    for (i = 0; i < neq; i++) Y1[i] = Y[i] + 0.5 * dt * k2[i];

    Ydot(t + 0.5 * dt, Y1, k3);
    for (i = 0; i < neq; i++) Y1[i] = Y[i] + dt * k3[i];

    Ydot(t + dt, Y1, k4);
    for (i = 0; i < neq; i++) {
        Y[i] += dt * (k1[i] + 2.0 * (k2[i] + k3[i]) + k4[i]) / 6.0;
    }
}


// Boris integrator
void Boris(double t, double *Y, double dt, int Npart)
{
    for (int p = 0; p < Npart; p++) {

        int i = 6 * p;

        double x  = Y[i + 0];
        double y  = Y[i + 1];
        double z  = Y[i + 2];

        double vx = Y[i + 3];
        double vy = Y[i + 4];
        double vz = Y[i + 5];

        double Ex, Ey, Ez;
        double Bx, By, Bz;

        ElectricField(x, y, z, t, Ex, Ey, Ez);
        MagneticField(x, y, z, t, Bx, By, Bz);

        // Half electric acceleration
        double qmdt2 = QOM * dt * 0.5;

        double vminus_x = vx + qmdt2 * Ex;
        double vminus_y = vy + qmdt2 * Ey;
        double vminus_z = vz + qmdt2 * Ez;

        // Magnetic rotation
        double tx = qmdt2 * Bx / C_LIGHT;
        double ty = qmdt2 * By / C_LIGHT;
        double tz = qmdt2 * Bz / C_LIGHT;

        double t2 = tx * tx + ty * ty + tz * tz;

        double sx = 2.0 * tx / (1.0 + t2);
        double sy = 2.0 * ty / (1.0 + t2);
        double sz = 2.0 * tz / (1.0 + t2);

        // v' = v- + v- x t
        double vprime_x = vminus_x + (vminus_y * tz - vminus_z * ty);
        double vprime_y = vminus_y + (vminus_z * tx - vminus_x * tz);
        double vprime_z = vminus_z + (vminus_x * ty - vminus_y * tx);

        // v+ = v- + v' x s
        double vplus_x = vminus_x + (vprime_y * sz - vprime_z * sy);
        double vplus_y = vminus_y + (vprime_z * sx - vprime_x * sz);
        double vplus_z = vminus_z + (vprime_x * sy - vprime_y * sx);

        // Second half electric acceleration
        vx = vplus_x + qmdt2 * Ex;
        vy = vplus_y + qmdt2 * Ey;
        vz = vplus_z + qmdt2 * Ez;

        // Position update
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;

        Y[i + 0] = x;
        Y[i + 1] = y;
        Y[i + 2] = z;

        Y[i + 3] = vx;
        Y[i + 4] = vy;
        Y[i + 5] = vz;
    }
}


// Velocity magnitude
double VelocityMagnitude(double vx, double vy, double vz)
{
    return sqrt(vx * vx + vy * vy + vz * vz);
}