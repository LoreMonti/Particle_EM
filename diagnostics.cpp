////////////////////////////////////////////////////////////////////////////////
// ---------------- Particle(s) in Electromagnetic Fields ------------------- //
////////////////////////////////////////////////////////////////////////////////
// This file contains the implementation of the diagnostics                   //
////////////////////////////////////////////////////////////////////////////////

#include "particle_em.h"

// ==========================================================
// Total kinetic energy
// ==========================================================
double KineticEnergy(const double Y[], int Npart)
{
    double energy = 0.0;

    for (int p = 0; p < Npart; p++) {
        const int i = 6 * p;

        const double vx = Y[i + 3];
        const double vy = Y[i + 4];
        const double vz = Y[i + 5];

        energy += 0.5 * (vx * vx + vy * vy + vz * vz);
    }

    return energy;
}

// ==========================================================
// Relative error with safe denominator
// ==========================================================
double RelativeError(double value, double reference)
{
    if (fabs(reference) < 1.0e-14) {
        return fabs(value - reference);
    }

    return fabs((value - reference) / reference);
}