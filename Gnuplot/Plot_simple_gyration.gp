# ==========================================================
# Plot script for simple gyration
# Compares RK4 and Boris on the same plots
# Saves all plots as PDF in the Plots directory
#
# Usage:
#   gnuplot Plot_simple_gyration.gp
# ==========================================================

set terminal pdfcairo enhanced color size 7,5

# ----------------------------------------------------------
# Input data files
# ----------------------------------------------------------
rk4  = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/simple_gyration_rk4.dat"
boris = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/simple_gyration_boris.dat"

rk4_diag   = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/simple_gyration_rk4_diagnostics.dat"
boris_diag = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/simple_gyration_boris_diagnostics.dat"

# Output directory (must already exist)
outdir = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Plots"

# Common style
set grid
set key top right

# ==========================================================
# Trajectory in the x-y plane
# ==========================================================
set output outdir . "/simple_gyration_trajectory_xy.pdf"

set title "Simple Gyration: Trajectory in the x-y Plane"
set xlabel "x"
set ylabel "y"
set size ratio -1
set xrange [0.5:3.5]
set yrange [-1.5:1.5]

plot \
    rk4   using 2:3 with lines lw 2 title "RK4", \
    boris using 2:3 with lines lw 2 dashtype 2 title "Boris"

# ==========================================================
# z(t)
# ==========================================================
set output outdir . "/simple_gyration_z_t.pdf"

set title "Simple Gyration: z(t)"
set xlabel "t"
set ylabel "z"
set size noratio

plot \
    rk4   using 1:4 with lines lw 2 title "RK4", \
    boris using 1:4 with lines lw 2 dashtype 2 title "Boris"

# ==========================================================
# Velocity magnitude
# ==========================================================
set output outdir . "/simple_gyration_velocity.pdf"

set title "Simple Gyration: Velocity Magnitude"
set xlabel "t"
set ylabel "|v|"

plot \
    rk4   using 1:8 with lines lw 2 title "RK4", \
    boris using 1:8 with lines lw 2 dashtype 2 title "Boris"

# ==========================================================
# Gyration radius from diagnostics
# ==========================================================
set output outdir . "/simple_gyration_radius.pdf"

set title "Simple Gyration: Distance from Origin"
set xlabel "t"
set ylabel "r = sqrt(x^{2} + y^{2})"

plot \
    rk4_diag   using 1:2 with lines lw 2 title "RK4", \
    boris_diag using 1:2 with lines lw 2 dashtype 2 title "Boris"

# ==========================================================
# Speed conservation from diagnostics
# ==========================================================
set output outdir . "/simple_gyration_speed_diagnostic.pdf"

set title "Simple Gyration: Speed Conservation"
set xlabel "t"
set ylabel "|v|"

plot \
    rk4_diag   using 1:3 with lines lw 2 title "RK4", \
    boris_diag using 1:3 with lines lw 2 dashtype 2 title "Boris"

# ----------------------------------------------------------
# Close output
# ----------------------------------------------------------
set output