# Raylib Solar System (2D)

A simple **2D solar system simulation** built with **raylib** (C++). Planets orbit around the sun with a camera-style **pan + zoom** view so you can explore the system.

## Features

- **2D orbital simulation**: Planets move on circular orbits (angle += speed * dt)
- **Pan + zoom**: Drag to pan, mouse wheel to zoom (clamped)
- **Labels + UI**: Planet names, zoom/offset readout, and control hints
- **CMake build**: Desktop build via CMake + raylib

## Controls

- **Left click + drag**: Pan view
- **Mouse wheel**: Zoom in/out
- **ESC**: Quit

## Building (Desktop / CMake)

From the repo root:

```bash
cmake -S . -B build
cmake --build build
```

The executable is produced in `build/` (for example `build/raylib_cmake_template.exe`).

## Project Structure

- `src/main.cpp`: Window + main loop
- `src/game.h`, `src/game.cpp`: Simulation, input (pan/zoom), and rendering
- `src/globals.h`, `src/globals.cpp`: Shared globals (includes render size: 800×600)
- `CMakeLists.txt`: CMake build configuration (raylib is pulled from `RAYLIB_PATH`)
- `data/`: Assets copied to the build directory (not required for the simulation visuals)

## Notes

- This project is intended as a straightforward raylib implementation of a 2D solar system viewer/sim.

## License

This project is licensed under the terms specified in the `LICENSE.txt` file.