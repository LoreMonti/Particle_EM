# ==========================================================
# Plot script for X-point configuration
# Compares RK4 and Boris on the same plots
# Saves all plots as PDF in the Plots directory
#
# Usage:
#   gnuplot Plot_xpoint.gp
# ==========================================================

set terminal pdfcairo enhanced color size 7,5

# ----------------------------------------------------------
# Input data files
# ----------------------------------------------------------
rk4   = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/xpoint_rk4.dat"
boris = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/xpoint_boris.dat"

rk4_diag   = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/xpoint_rk4_diagnostics.dat"
boris_diag = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/xpoint_boris_diagnostics.dat"

# Output directory
outdir = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Plots"

set grid
set key top right

# ==========================================================
# Particle positions in the x-y plane
# Columns:
# 1=t, 2=particle_id, 3=x, 4=y, 5=z, 6=vx, 7=vy, 8=vz, 9=|v|
# ==========================================================
set output outdir . "/xpoint_particles_xy.pdf"

set title "X-point: Particle Positions in the x-y Plane"
set xlabel "x"
set ylabel "y"
set size ratio -1
unset key

plot \
    rk4   using 3:4 with dots title "RK4", \
    boris using 3:4 with dots title "Boris"

# ==========================================================
# Particle positions in the x-z plane
# ==========================================================
set output outdir . "/xpoint_particles_xz.pdf"

set title "X-point: Particle Positions in the x-z Plane"
set xlabel "x"
set ylabel "z"
set size ratio -1

plot \
    rk4   using 3:5 with dots title "RK4", \
    boris using 3:5 with dots title "Boris"

# ==========================================================
# Velocity magnitude distribution
# ==========================================================
set output outdir . "/xpoint_velocity_distribution.pdf"

set title "X-point: Velocity Magnitude"
set xlabel "t"
set ylabel "|v|"
set size noratio

plot \
    rk4   using 1:9 with dots title "RK4", \
    boris using 1:9 with dots title "Boris"

# ==========================================================
# Mean velocity magnitude
# Diagnostics columns:
# 1=t, 2=v_mean, 3=v_max, 4=z_mean, 5=z_max,
# 6=x_vmax, 7=y_vmax, 8=z_vmax
# ==========================================================
set output outdir . "/xpoint_mean_velocity.pdf"

set title "X-point: Mean Velocity Magnitude"
set xlabel "t"
set ylabel "<|v|>"
set key top left

plot \
    rk4_diag   using 1:2 with lines lw 2 title "RK4", \
    boris_diag using 1:2 with lines lw 2 dashtype 2 title "Boris"

# ==========================================================
# Mean z position
# ==========================================================
set output outdir . "/xpoint_mean_z.pdf"

set title "X-point: Mean z Position"
set xlabel "t"
set ylabel "<z>"

plot \
    rk4_diag   using 1:4 with lines lw 2 title "RK4", \
    boris_diag using 1:4 with lines lw 2 dashtype 2 title "Boris"

# ----------------------------------------------------------
# Close output
# ----------------------------------------------------------
set output