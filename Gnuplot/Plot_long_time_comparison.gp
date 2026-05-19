# ==========================================================
# Long-time RK4 vs Boris comparison
# Uses diagnostics files from simple gyration
# ==========================================================

set terminal pdfcairo enhanced color size 7,5

rk4_diag   = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/simple_gyration_rk4_diagnostics.dat"
boris_diag = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Data/simple_gyration_boris_diagnostics.dat"

outdir = "/Users/lorenzo/Desktop/GitHub/Particle_EM/Plots"

set grid
set key top left

# Columns:
# 1 = t
# 2 = r_xy
# 3 = v_abs
# 4 = z
# 5 = energy
# 6 = rel_energy_error
# 7 = rel_speed_error

# ----------------------------------------------------------
# Relative energy error
# ----------------------------------------------------------
set output outdir . "/long_time_energy_error.pdf"

set title "Long-Time Energy Conservation: RK4 vs Boris"
set xlabel "t"
set ylabel "Relative kinetic energy error"
set logscale y
set format y "10^{%T}"

plot \
    rk4_diag   using 1:6 with lines lw 2 title "RK4", \
    boris_diag using 1:6 with lines lw 2 dashtype 2 title "Boris"

unset logscale y
unset format y

# ----------------------------------------------------------
# Speed magnitude
# ----------------------------------------------------------
set output outdir . "/long_time_speed_comparison.pdf"

set title "Long-Time Speed Magnitude: RK4 vs Boris"
set xlabel "t"
set ylabel "|v|"

plot \
    rk4_diag   using 1:3 with lines lw 2 title "RK4", \
    boris_diag using 1:3 with lines lw 2 dashtype 2 title "Boris"

# ----------------------------------------------------------
# Kinetic energy
# ----------------------------------------------------------
set output outdir . "/long_time_energy_comparison.pdf"

set title "Long-Time Kinetic Energy: RK4 vs Boris"
set xlabel "t"
set ylabel "Kinetic energy"

plot \
    rk4_diag   using 1:5 with lines lw 2 title "RK4", \
    boris_diag using 1:5 with lines lw 2 dashtype 2 title "Boris"

# ----------------------------------------------------------
# Distance from origin in x-y plane
# ----------------------------------------------------------
set output outdir . "/long_time_rxy_comparison.pdf"

set title "Long-Time Distance from Origin in the x-y Plane"
set xlabel "t"
set ylabel "r_{xy} = sqrt(x^{2} + y^{2})"

plot \
    rk4_diag   using 1:2 with lines lw 2 title "RK4", \
    boris_diag using 1:2 with lines lw 2 dashtype 2 title "Boris"

set output