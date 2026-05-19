///////////////////////////////////////////////////////////////////////////////
// --------------- Particle(s) in Electromagnetic Fields -------------------- //
///////////////////////////////////////////////////////////////////////////////
// This file contains the main program and the high-level simulation workflow //
//                                                                            //
// The main routine performs the following tasks:                             //
//   1. Creates output folders.                                               //
//   2. Selects the number of particles according to the physical case.       //
//   3. Initializes the state vector.                                         //
//   4. Opens data and diagnostic output files.                               //
//   5. Advances the system in time using the selected numerical method.      //
//   6. Writes simulation outputs and final diagnostics.                      //
///////////////////////////////////////////////////////////////////////////////
// ----------------------------- Lorenzo Monti ------------------------------ //
///////////////////////////////////////////////////////////////////////////////
// -------------------------------- 05/2026 --------------------------------- //
///////////////////////////////////////////////////////////////////////////////
// Compilation example:
// g++ main.cpp fields.cpp integrators.cpp initialization.cpp output.cpp -o main
///////////////////////////////////////////////////////////////////////////////

#include "particle_em.h"

// ==========================================================
// Main program
// ==========================================================
int main()
{
    // ======================================================
    // 1. Simulation setup
    // ======================================================

    CreateOutputFolders();

    int Npart = 1;

#if CASE == CASE_X_POINT
    Npart = NPART_XPOINT;
#endif

    const int neq = 6 * Npart;

    if (neq > NMAX_EQ) {
        cerr << "Error: number of equations exceeds NMAX_EQ." << endl;
        cerr << "neq = " << neq << ", NMAX_EQ = " << NMAX_EQ << endl;
        return 1;
    }

    const double tb = TB;
    const double te = TE;
    const double dt = (te - tb) / double(NSTEPS);

    double t = tb;

    // ======================================================
    // 2. Print run information
    // ======================================================

    cout << "============================================" << endl;
    cout << "Particle(s) in Electromagnetic Fields" << endl;
    cout << "============================================" << endl;

#if CASE == CASE_SIMPLE_GYRATION
    cout << "Physical case: Simple gyration" << endl;

    #if TEST_CASE == GYRATION_CIRCLE
    cout << "Gyration type: Circular orbit" << endl;
    #elif TEST_CASE == GYRATION_HELIX
    cout << "Gyration type: Helical orbit" << endl;
    #endif

#elif CASE == CASE_EXB_DRIFT
    cout << "Physical case: E x B drift" << endl;

#elif CASE == CASE_X_POINT
    cout << "Physical case: X-point" << endl;
#endif

#if METHOD == METHOD_RK4
    cout << "Numerical method: RK4" << endl;
#elif METHOD == METHOD_BORIS
    cout << "Numerical method: Boris" << endl;
#endif

    cout << "Number of particles: " << Npart << endl;
    cout << "Number of equations: " << neq << endl;
    cout << "Number of steps: " << NSTEPS << endl;
    cout << "Output every: " << OUTPUT_EVERY << " step(s)" << endl;
    cout << "Time interval: [" << tb << ", " << te << "]" << endl;
    cout << "dt = " << dt << endl;
    cout << "============================================" << endl;

    // ======================================================
    // 3. Allocate and initialize the state vector
    // ======================================================
    //
    // State-vector layout for particle p:
    //
    //   Y[6*p + 0] = x
    //   Y[6*p + 1] = y
    //   Y[6*p + 2] = z
    //   Y[6*p + 3] = vx
    //   Y[6*p + 4] = vy
    //   Y[6*p + 5] = vz
    //
    // The full system therefore contains 6 equations per particle.
    // ======================================================

    double Y[NMAX_EQ];

    // Use a fixed seed for reproducible runs.
    // Replace with srand(time(NULL)) for stochastic runs.
    srand(12345);
    // srand(time(NULL));

    InitializeParticles(Y, Npart);

    const double energy0 = KineticEnergy(Y, Npart);

    // ======================================================
    // 4. Open output files
    // ======================================================

    ofstream fdata;
    ofstream fdiag;

    OpenOutputFiles(fdata, fdiag);

    if (!fdata.is_open()) {
        cerr << "Error: could not open output data file." << endl;
        return 1;
    }

    if (!fdiag.is_open()) {
        cerr << "Error: could not open diagnostics file." << endl;
        return 1;
    }

    fdata << scientific << setprecision(8);
    fdiag << scientific << setprecision(8);

    // ======================================================
    // 5. Write output headers
    // ======================================================

#if CASE == CASE_X_POINT
    fdata << "# t particle_id x y z vx vy vz v_abs" << endl;
#else
    fdata << "# t x y z vx vy vz v_abs" << endl;
#endif

#if CASE == CASE_SIMPLE_GYRATION
    fdiag << "# t r_xy v_abs z energy rel_energy_error rel_speed_error" << endl;
#elif CASE == CASE_EXB_DRIFT
    fdiag << "# t x y v_abs v_drift_theory energy rel_energy_error rel_speed_error" << endl;
#elif CASE == CASE_X_POINT
    fdiag << "# t v_mean v_max z_mean z_max x_vmax y_vmax z_vmax energy rel_energy_error" << endl;
#endif

    // Save initial condition.
    WriteOutput(fdata, t, Y, Npart);

    // ======================================================
    // 6. Time integration loop
    // ======================================================

    int last_percent = -1;

    for (int istep = 0; istep < NSTEPS; istep++) {

        // Advance particle state by one time step.
#if METHOD == METHOD_RK4
        RK4(t, Y, dYdt, dt, neq);
#elif METHOD == METHOD_BORIS
        Boris(t, Y, dt, Npart);
#endif

        t += dt;

        // Write main simulation output at the selected cadence.
        if (istep % OUTPUT_EVERY == 0) {
            WriteOutput(fdata, t, Y, Npart);
        }

        // Write case-specific diagnostics.
#if CASE == CASE_SIMPLE_GYRATION

        const double r_xy  = sqrt(Y[0] * Y[0] + Y[1] * Y[1]);
        const double v_abs = VelocityMagnitude(Y[3], Y[4], Y[5]);

        const double energy = KineticEnergy(Y, Npart);
        const double rel_energy_error = RelativeError(energy, energy0);

        const double speed0 = VelocityMagnitude(Y[3], Y[4], Y[5]);
        const double rel_speed_error = RelativeError(v_abs, speed0);

        fdiag << t << " "
            << r_xy << " "
            << v_abs << " "
            << Y[2] << " "
            << energy << " "
            << rel_energy_error << " "
            << rel_speed_error << endl;

#elif CASE == CASE_EXB_DRIFT

        const double v_abs = VelocityMagnitude(Y[3], Y[4], Y[5]);
        const double v_drift_theory = E0;

        const double energy = KineticEnergy(Y, Npart);
        const double rel_energy_error = RelativeError(energy, energy0);
        const double rel_speed_error  = RelativeError(v_abs, speed0);

        fdiag << t << " "
            << Y[0] << " "
            << Y[1] << " "
            << v_abs << " "
            << v_drift_theory << " "
            << energy << " "
            << rel_energy_error << " "
            << rel_speed_error << endl;

#elif CASE == CASE_X_POINT

        double v_sum = 0.0;
        double z_sum = 0.0;

        double v_max = 0.0;
        double z_max = 0.0;

        double x_vmax = 0.0;
        double y_vmax = 0.0;
        double z_vmax = 0.0;

        for (int p = 0; p < Npart; p++) {
            const int i = 6 * p;

            const double v_abs = VelocityMagnitude(Y[i + 3],
                                                   Y[i + 4],
                                                   Y[i + 5]);

            v_sum += v_abs;
            z_sum += Y[i + 2];

            if (v_abs > v_max) {
                v_max = v_abs;
                x_vmax = Y[i + 0];
                y_vmax = Y[i + 1];
                z_vmax = Y[i + 2];
            }

            if (fabs(Y[i + 2]) > fabs(z_max)) {
                z_max = Y[i + 2];
            }
        }

        const double v_mean = v_sum / double(Npart);
        const double z_mean = z_sum / double(Npart);

        const double energy = KineticEnergy(Y, Npart);
        const double rel_energy_error = RelativeError(energy, energy0);

        fdiag << t << " "
            << v_mean << " "
            << v_max << " "
            << z_mean << " "
            << z_max << " "
            << x_vmax << " "
            << y_vmax << " "
            << z_vmax << " "
            << energy << " "
            << rel_energy_error << endl;

#endif

        // Print progress percentage.
        const int percent = int((t - tb) / (te - tb) * 100.0);

        if (percent > last_percent) {
            last_percent = percent;
            cout << "\r" << percent << "% completed " << flush;
        }
    }

    // ======================================================
    // 7. Close files and print final diagnostics
    // ======================================================

    fdata.close();
    fdiag.close();

    cout << endl;

    const string case_tag = GetCaseTag();
    const string method_tag = GetMethodTag();

    cout << "-> Data written to Data/"
         << case_tag << "_" << method_tag << ".dat" << endl;

    cout << "-> Diagnostics written to Data/"
         << case_tag << "_" << method_tag << "_diagnostics.dat" << endl;

    PrintFinalDiagnostics(tb, te, dt, Y, Npart);

    cout << "Simulation completed successfully." << endl;

    return 0;
}
