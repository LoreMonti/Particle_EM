////////////////////////////////////////////////////////////////////////////////
// ---------------- Particle(s) in Electromagnetic Fields ------------------- //
////////////////////////////////////////////////////////////////////////////////
// This file contains particle initialization and random utilities.           //
////////////////////////////////////////////////////////////////////////////////

#include "particle_em.h"

// Initialization of particles
void InitializeParticles(double *Y, int Npart)
{
    (void)Npart;
    
#if CASE == CASE_SIMPLE_GYRATION

    Y[0] = 1.0;   // x
    Y[1] = 0.0;   // y
    Y[2] = 0.0;   // z

    Y[3] = 0.0;   // vx
    Y[4] = 1.0;   // vy

    #if TEST_CASE == GYRATION_CIRCLE
    Y[5] = 0.0;   // vz: circular orbit in the x-y plane
    #elif TEST_CASE == GYRATION_HELIX
    Y[5] = 0.5;   // vz: helical motion
    #endif

#elif CASE == CASE_EXB_DRIFT

    Y[0] = 1.0;   // x
    Y[1] = 0.0;   // y
    Y[2] = 0.0;   // z

    Y[3] = 0.0;   // vx
    Y[4] = 1.0;   // vy
    Y[5] = 0.0;   // vz

#elif CASE == CASE_X_POINT

    for (int p = 0; p < Npart; p++) {

        int i = 6 * p;

        // Uniform initial position in [-L, L] x [-L, L]
        Y[i + 0] = RandomUniform(-L_XPOINT, L_XPOINT);  // x
        Y[i + 1] = RandomUniform(-L_XPOINT, L_XPOINT);  // y
        Y[i + 2] = 0.0;                                 // z

        // Random velocity direction with fixed magnitude V0_XPOINT
        double theta = RandomUniform(0.0, 2.0 * M_PI);
        double mu    = RandomUniform(-1.0, 1.0);

        double sin_phi = sqrt(1.0 - mu * mu);

        Y[i + 3] = V0_XPOINT * sin_phi * cos(theta);    // vx
        Y[i + 4] = V0_XPOINT * sin_phi * sin(theta);    // vx
        Y[i + 5] = V0_XPOINT * mu;                      // vz
    }

#endif
}


// Random number uniformly distributed in [a,b]
double RandomUniform(double a, double b)
{
    return a + (b - a) * double(rand()) / double(RAND_MAX);
}
