////////////////////////////////////////////////////////////////////////////////
// ---------------- Particle(s) in Electromagnetic Fields -------------------- //
////////////////////////////////////////////////////////////////////////////////
// This file contains output routines and diagnostic functions.               //
////////////////////////////////////////////////////////////////////////////////

#include "particle_em.h"

// Create output folders
void CreateOutputFolders()
{
    mkdir("Data", 0777);
    mkdir("Plots", 0777);
}


// Write simulation output
void WriteOutput(ofstream &fdata, double t, double *Y, int Npart)
{
    (void)Npart;
    
#if CASE == CASE_X_POINT

    for (int p = 0; p < Npart; p++) {

        int i = 6 * p;

        double v_abs = VelocityMagnitude(Y[i + 3], Y[i + 4], Y[i + 5]);

        fdata << t << " "
              << p << " "
              << Y[i + 0] << " " << Y[i + 1] << " " << Y[i + 2] << " "
              << Y[i + 3] << " " << Y[i + 4] << " " << Y[i + 5] << " "
              << v_abs << endl;
    }

    fdata << endl;

#else

    double v_abs = VelocityMagnitude(Y[3], Y[4], Y[5]);

    fdata << t << " "
          << Y[0] << " " << Y[1] << " " << Y[2] << " "
          << Y[3] << " " << Y[4] << " " << Y[5] << " "
          << v_abs << endl;

#endif
}


// Open output files with case and method in the filename
void OpenOutputFiles(ofstream &fdata, ofstream &fdiag)
{
    string case_tag = GetCaseTag();
    string method_tag = GetMethodTag();

    string data_name = "Data/" + case_tag + "_" + method_tag + ".dat";
    string diag_name = "Data/" + case_tag + "_" + method_tag + "_diagnostics.dat";

    fdata.open(data_name.c_str());
    fdiag.open(diag_name.c_str());
}



// Print quantitative diagnostics at the end of the simulation
void PrintFinalDiagnostics(double tb, double te, double dt, double *Y, int Npart)
{
    (void)tb;
    (void)te;
    (void)dt;
    (void)Npart;

    cout << "============================================" << endl;
    cout << "Final quantitative diagnostics" << endl;
    cout << "============================================" << endl;

#if CASE == CASE_SIMPLE_GYRATION

    double v_abs = VelocityMagnitude(Y[3], Y[4], Y[5]);
    double r_origin = sqrt(Y[0] * Y[0] + Y[1] * Y[1]);

    double v_perp = sqrt(Y[3] * Y[3] + Y[4] * Y[4]);
    double r_larmor_theory = v_perp / 1.0;

    cout << "Final position: "
         << "x = " << Y[0] << ", "
         << "y = " << Y[1] << ", "
         << "z = " << Y[2] << endl;

    cout << "Final velocity: "
         << "vx = " << Y[3] << ", "
         << "vy = " << Y[4] << ", "
         << "vz = " << Y[5] << endl;

    cout << "Final |v| = " << v_abs << endl;
    cout << "Final distance from origin sqrt(x^2+y^2) = " << r_origin << endl;
    cout << "Theoretical Larmor radius r_L = v_perp / B = "
         << r_larmor_theory << endl;

#elif CASE == CASE_EXB_DRIFT

    double v_abs = VelocityMagnitude(Y[3], Y[4], Y[5]);

    double x_theory = 1.0 + E0 * (te - tb);
    double drift_error = Y[0] - x_theory;
    double relative_error = fabs(drift_error) / fabs(x_theory);

    cout << "Final position: "
         << "x = " << Y[0] << ", "
         << "y = " << Y[1] << ", "
         << "z = " << Y[2] << endl;

    cout << "Final velocity: "
         << "vx = " << Y[3] << ", "
         << "vy = " << Y[4] << ", "
         << "vz = " << Y[5] << endl;

    cout << "Final |v| = " << v_abs << endl;
    cout << "Theoretical drift velocity v_ExB = E0 = " << E0 << endl;
    cout << "Theoretical final x = x0 + E0 * t = " << x_theory << endl;
    cout << "Drift error x_num - x_theory = " << drift_error << endl;
    cout << "Relative drift error = " << relative_error << endl;

#elif CASE == CASE_X_POINT

    double v_sum = 0.0;
    double z_sum = 0.0;

    double v_max = 0.0;
    double z_max_abs = 0.0;

    int p_vmax = 0;
    int p_zmax = 0;

    for (int p = 0; p < Npart; p++) {
        int i = 6 * p;

        double v_abs = VelocityMagnitude(Y[i + 3], Y[i + 4], Y[i + 5]);

        v_sum += v_abs;
        z_sum += Y[i + 2];

        if (v_abs > v_max) {
            v_max = v_abs;
            p_vmax = p;
        }

        if (fabs(Y[i + 2]) > fabs(z_max_abs)) {
            z_max_abs = Y[i + 2];
            p_zmax = p;
        }
    }

    double v_mean = v_sum / double(Npart);
    double z_mean = z_sum / double(Npart);

    int iv = 6 * p_vmax;
    int iz = 6 * p_zmax;

    cout << "Mean final |v| = " << v_mean << endl;
    cout << "Maximum final |v| = " << v_max
         << " for particle " << p_vmax << endl;

    cout << "Position of fastest particle: "
         << "x = " << Y[iv + 0] << ", "
         << "y = " << Y[iv + 1] << ", "
         << "z = " << Y[iv + 2] << endl;

    cout << "Mean final z = " << z_mean << endl;
    cout << "Maximum |z| = " << z_max_abs
         << " for particle " << p_zmax << endl;

    cout << "Position of particle with maximum |z|: "
         << "x = " << Y[iz + 0] << ", "
         << "y = " << Y[iz + 1] << ", "
         << "z = " << Y[iz + 2] << endl;

#endif

    cout << "============================================" << endl;
}


// Return method name used in output files
string GetMethodTag()
{
#if METHOD == METHOD_RK4
    return "rk4";
#elif METHOD == METHOD_BORIS
    return "boris";
#endif
}


// Return physical case name used in output files
string GetCaseTag()
{
#if CASE == CASE_SIMPLE_GYRATION
    return "simple_gyration";
#elif CASE == CASE_EXB_DRIFT
    return "exb_drift";
#elif CASE == CASE_X_POINT
    return "xpoint";
#endif
}
