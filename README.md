# Particle(s) in Electromagnetic Fields

The code numerically solves the non-relativistic Lorentz force equation for charged particles moving in prescribed electromagnetic fields using either a fourth-order Runge–Kutta method or the Boris particle pusher. It supports several benchmark configurations, including simple gyration, ExB drift, and a magnetic X-point.

---

## Overview

The equation of motion is

$$
\frac{d\mathbf{x}}{dt} = \mathbf{v}
\qquad \qquad
\frac{d\mathbf{v}}{dt} =
\frac{q}{m}
\left(
\mathbf{E} + \frac{\mathbf{v}}{c} \times \mathbf{B}
\right)
$$

where:

- $$\mathbf{x}$$ is the particle position,
- $$\mathbf{v}$$ is the particle velocity,
- $$q/m$$ is the charge-to-mass ratio,
- $$\mathbf{E}$$ is the electric field,
- $$\mathbf{B}$$ is the magnetic field,
- $$c$$ is the speed of light in normalized units.

The code uses normalized units by default.

---

## Features

- Non-relativistic charged particle dynamics
- Single-particle and multi-particle simulations
- Runge-Kutta 4th order integrator
- Boris particle pusher
- Uniform magnetic-field gyration test
- ExB drift test
- Magnetic X-point configuration
- Case-dependent diagnostics
- Gnuplot-compatible output files
- Modular C++ project structure
- Reproducible compilation through a Makefile
- Python-based 3D GIF animations
- Automated generation of animated particle trajectories
- Long-time diagnostics for energy and speed conservation

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
├── README.md
├── LICENSE
├── .gitignore
├── Data/
├── Plots/
└── Gnuplot/
````

### File Description

| File                 | Description                                                             |
| -------------------- | ----------------------------------------------------------------------- |
| `main.cpp`           | Main simulation workflow                                                |
| `particle_em.h`      | Global parameters, case selection, constants, and function declarations |
| `fields.cpp`         | Electric and magnetic field definitions                                 |
| `integrators.cpp`    | Equations of motion and numerical integrators                           |
| `initialization.cpp` | Particle initialization and random utilities                            |
| `output.cpp`         | Output files, diagnostics, and final summaries                          |
| `Makefile`           | Compilation and cleaning commands                                       |
| `Gnuplot/`           | Plotting scripts                                                        |
| `Data/`              | Simulation output data                                                  |
| `Plots/`             | Generated figures                                                       |

---

## Physical Configurations

The physical case is selected in `particle_em.h` using:

```cpp
#define CASE CASE_X_POINT
```

Available cases are:

```cpp
#define CASE_SIMPLE_GYRATION 1
#define CASE_EXB_DRIFT       2
#define CASE_X_POINT         3
```

---

### 1. Simple Gyration

This case simulates the motion of a charged particle in a uniform magnetic field:

```text
E = 0
B = B0 z_hat
```

The particle performs circular or helical motion depending on the initial parallel velocity.

This case is useful for validating:

* Larmor rotation,
* conservation of perpendicular speed,
* numerical stability of the integrator,
* long-time behavior in a purely magnetic field.

The gyration type is selected with:

```cpp
#define TEST_CASE GYRATION_CIRCLE
```

or

```cpp
#define TEST_CASE GYRATION_HELIX
```

---

### 2. ExB Drift

This case simulates a charged particle in crossed electric and magnetic fields:

```text
E = E0 y_hat
B = B0 z_hat
```

The expected guiding-center drift is the ExB drift:

```text
v_drift = E x B / B^2
```

In the normalized setup used here, the theoretical drift velocity is controlled by `E0`.

This case is useful for testing whether the numerical integrator correctly reproduces the expected drift motion.

---

### 3. Magnetic X-point

This case simulates an ensemble of particles in a two-dimensional magnetic X-point configuration.

The magnetic field has the form:

```text
Bx =  y / L
By = -x / L
Bz =  0
```

An electric field can be imposed to accelerate particles along the out-of-plane direction.

This configuration is useful for studying:

* particle acceleration,
* magnetic null-point dynamics,
* multi-particle statistical behavior,
* velocity growth and spatial spreading.

For this case, the number of particles is controlled by:

```cpp
const int NPART_XPOINT = 1000;
```

---

## Numerical Integrators

The numerical method is selected in `particle_em.h`:

```cpp
#define METHOD METHOD_BORIS
```

Available methods are:

```cpp
#define METHOD_RK4    1
#define METHOD_BORIS  2
```

---

### Runge-Kutta 4th Order

The RK4 method is a classical high-order explicit integrator.

It is useful because:

* it is general-purpose,
* it has fourth-order accuracy in time,
* it is simple to compare against other methods.

However, for charged-particle motion in magnetic fields, RK4 is not structure-preserving. Over long integrations, it may introduce artificial energy errors, especially in purely magnetic problems.

---

### Boris Particle Pusher

The Boris method is widely used in plasma physics and particle-in-cell simulations.

It is particularly suitable for charged-particle dynamics because:

* it handles magnetic rotation very well,
* it is time-centered,
* it has excellent long-term stability,
* it avoids artificial energy growth in purely magnetic fields,
* it is computationally efficient.

For this reason, the Boris method is usually the preferred choice for long-time charged-particle simulations.

---

## Run

Compile and execute the simulation with:

```bash
make run
```

---

## Output Files

Simulation data are written to the `Data/` directory.

Example files:

```text
Data/simple_gyration_boris.dat
Data/exb_drift_boris.dat
Data/xpoint_boris.dat
```

Diagnostics are written as:

```text
Data/simple_gyration_boris_diagnostics.dat
Data/exb_drift_boris_diagnostics.dat
Data/xpoint_boris_diagnostics.dat
```

The output filename automatically depends on the selected physical case and numerical method.

---

## Plotting and Visualization

Static diagnostic plots can be generated using Gnuplot scripts stored in the `Gnuplot/` directory.

Example:

```bash
gnuplot Gnuplot/Plot_xpoint.gp
````

Animated three-dimensional GIFs can be generated with the Python visualization script:

```bash
python Scripts/plot_3d_gifs.py
```

Generated figures and animations are saved in the `Plots/` directory.

---

## Typical Workflow

1. Select the physical case in `particle_em.h`.
2. Select the numerical method in `particle_em.h`.
3. Compile the code and run the simulation:

```bash
make run
```

4. Generate plots with Gnuplot.
5. Inspect data, diagnostics, and final terminal output.

---

## Repository Notes

Generated data and plots are not tracked by Git.

The folders `Data/` and `Plots/` are kept in the repository using `.gitkeep` files, while generated files are ignored through `.gitignore`.

---

## Possible Extensions

Future improvements may include:

* relativistic particle dynamics,
* adaptive time stepping,
* OpenMP parallelization,
* unit tests,
* additional electromagnetic field configurations,

---

## License

This project is released under the MIT License.

---

## Author

Lorenzo Monti,
May 2026
