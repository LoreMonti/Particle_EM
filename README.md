# Particle(s) in Electromagnetic Fields

Numerical simulation of charged particle motion in prescribed electromagnetic fields.

This project solves the non-relativistic Lorentz force equation using two widely used numerical integrators:

- Runge-Kutta 4th order (RK4)
- Boris particle pusher

The code includes several benchmark configurations commonly used in computational plasma physics.

---

## Features

- Non-relativistic charged particle dynamics
- Single-particle and multi-particle simulations
- RK4 and Boris integrators
- Simple gyration in a uniform magnetic field
- ExB drift in crossed electric and magnetic fields
- Magnetic X-point configuration
- Quantitative diagnostics
- Gnuplot-compatible output files

---

## Project Structure

```text
Particle_EM/
├── main.cpp
├── particle_em.h
├── fields.cpp
├── integrators.cpp
├── initialization.cpp
├── output.cpp
├── Makefile
├── .gitignore
├── LICENSE
├── README.md
├── Data/
└── Plots/