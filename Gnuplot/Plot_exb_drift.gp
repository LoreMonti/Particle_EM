# ==========================================================
# Plot script for E x B Drift
# Compares RK4 and Boris on the same plots
# Saves all plots as PDF in the Plots directory
#
# Usage:
#   gnuplot Plot_exb_drift.gp
# ==========================================================

set terminal pdfcairo enhanced color size 7,5

# ----------------------------------------------------------
# Input data files
# ----------------------------------------------------------
rk4   = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/exb_drift_rk4.dat"
boris = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/exb_drift_boris.dat"

rk4_diag   = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/exb_drift_rk4_diagnostics.dat"
boris_diag = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/exb_drift_boris_diagnostics.dat"

# Output directory
outdir = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Plots"

set grid
set key top right

# ==========================================================
# Trajectory in the x-y plane
# ==========================================================
set output outdir . "/exb_drift_trajectory_xy.pdf"

set title "E x B Drift: Trajectory in the x-y Plane"
set xlabel "x"
set ylabel "y"
set size ratio -1

plot \
    rk4   using 2:3 with lines lw 2 title "RK4", \
    boris using 2:3 with lines lw 2 dashtype 2 title "Boris"

# ==========================================================
# Velocity magnitude
# ==========================================================
set output outdir . "/exb_drift_velocity.pdf"

set title "E x B Drift: Velocity Magnitude"
set xlabel "t"
set ylabel "|v|"

plot \
    rk4   using 1:8 with lines lw 2 title "RK4", \
    boris using 1:8 with lines lw 2 dashtype 2 title "Boris"

# ==========================================================
# x(t) compared with theoretical E x B drift
# Diagnostics columns:
# 1 = t, 2 = x, 3 = y, 4 = |v|, 5 = v_drift_theory
# ==========================================================
set output outdir . "/exb_drift_theory_comparison.pdf"

set title "E x B Drift: x(t) compared with theoretical drift"
set xlabel "t"
set ylabel "x"
set size noratio

plot \
    rk4_diag   using 1:2 with lines lw 2 title "RK4", \
    boris_diag using 1:2 with lines lw 2 dashtype 2 title "Boris", \
    rk4_diag   using 1:($5*$1 + 1.0) with lines lw 2 dashtype 3 title "x_0 + E_0 t"

# ----------------------------------------------------------
# Close output
# ----------------------------------------------------------
set output