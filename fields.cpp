////////////////////////////////////////////////////////////////////////////////
// ---------------- Particle(s) in Electromagnetic Fields ------------------- //
////////////////////////////////////////////////////////////////////////////////
// This file contains the implementation of electromagnetic field definitions //
////////////////////////////////////////////////////////////////////////////////

#include "particle_em.h"

// Electric field
void ElectricField(double x, double y, double z, double t,
                   double &Ex, double &Ey, double &Ez)
{
    (void)x;
    (void)y;
    (void)z;
    (void)t;

#if CASE == CASE_SIMPLE_GYRATION

    Ex = 0.0;
    Ey = 0.0;
    Ez = 0.0;

#elif CASE == CASE_EXB_DRIFT

    Ex = 0.0;
    Ey = E0;
    Ez = 0.0;

#elif CASE == CASE_X_POINT

    Ex = 0.0;
    Ey = 0.0;
    Ez = 1.0;

#endif
}


// Magnetic field
void MagneticField(double x, double y, double z, double t,
                   double &Bx, double &By, double &Bz)
{
    (void)x;
    (void)y;
    (void)z;
    (void)t;

#if CASE == CASE_SIMPLE_GYRATION

    Bx = 0.0;
    By = 0.0;
    Bz = 1.0;

#elif CASE == CASE_EXB_DRIFT

    Bx = 0.0;
    By = 0.0;
    Bz = 1.0;

#elif CASE == CASE_X_POINT

    Bx = y / L_XPOINT;
    By = x / L_XPOINT;
    Bz = 0.0;

#endif
}