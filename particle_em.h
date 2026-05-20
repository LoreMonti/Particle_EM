////////////////////////////////////////////////////////////////////////////////
// ---------------- Particle(s) in Electromagnetic Fields -------------------- //
////////////////////////////////////////////////////////////////////////////////
// This header file contains global parameters and function declarations.     //
////////////////////////////////////////////////////////////////////////////////

#ifndef PARTICLE_EM_H
#define PARTICLE_EM_H

#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>

using namespace std;

// ==========================================================
// User choices
// ==========================================================

// Physical case
#define CASE_SIMPLE_GYRATION 1
#define CASE_EXB_DRIFT       2
#define CASE_X_POINT         3

#define CASE CASE_X_POINT

// Numerical method
#define METHOD_RK4    1
#define METHOD_BORIS  2

#define METHOD METHOD_BORIS

// Gyration type
#define GYRATION_CIRCLE 1
#define GYRATION_HELIX  2

#define TEST_CASE GYRATION_HELIX

// ==========================================================
// Code limits
// ==========================================================
#define NMAX_EQ 6000

// ==========================================================
// Physical parameters
// ==========================================================
const double QOM     = 1.0;
const double C_LIGHT = 1.0;
const double E0      = 0.2;

// ==========================================================
// Simulation parameters
// ==========================================================
const double TB = 0.0;
const double TE = 200.0;
const int NSTEPS = 20000;
const int OUTPUT_EVERY = 20;

// ==========================================================
// X-point parameters
// ==========================================================
const int NPART_XPOINT = 1000;
const double L_XPOINT  = 1.0e3;
const double V0_XPOINT = 0.1;

// ==========================================================
// Function prototypes
// ==========================================================

// Fields
void ElectricField(double x, double y, double z, double t,
                   double &Ex, double &Ey, double &Ez);

void MagneticField(double x, double y, double z, double t,
                   double &Bx, double &By, double &Bz);

// Equations of motion
void dYdt(double t, double *Y, double *R);

// Integrators
void RK4(double t, double *Y,
         void (*Ydot)(double, double *, double *),
         double dt, int neq);

void Boris(double t, double *Y, double dt, int Npart);

// Initialization
void InitializeParticles(double *Y, int Npart);
double RandomUniform(double a, double b);

// Output
void CreateOutputFolders();
void WriteOutput(ofstream &fdata, double t, double *Y, int Npart);
void OpenOutputFiles(ofstream &fdata, ofstream &fdiag);
void PrintFinalDiagnostics(double tb, double te, double dt,
                           double *Y, int Npart);

// Utilities
string GetMethodTag();
string GetCaseTag();
double VelocityMagnitude(double vx, double vy, double vz);

// Diagnostics
double KineticEnergy(const double Y[], int Npart);
double RelativeError(double value, double reference);

#endif