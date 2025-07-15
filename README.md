# Autonomous Car GPU Scheduler

This project simulates a GPU scheduling algorithm (Longest Processing Time first, LPT) for an autonomous car's main computation modules.

## Modules simulated:
- Perception (Sensor Fusion)
- Localization (Position Estimation)
- Object Detection
- Path Planning
- Control (Actuator Commands)

## Features
- Assigns the heaviest (longest) remaining module to the next available GPU.
- Console output shows scheduling decisions and progress.
- Reports makespan and theoretical lower bound for optimality reference.

## Build Instructions

**Requirements:**  
- GCC or any standard C compiler
- Make (recommended)

**Build:**
```sh
make
```

**Run:**
```sh
./scheduler
```

## Files

- `main.c` — Entry point, sets up modules and runs scheduler.
- `scheduler.c` — Scheduling logic and simulation.
- `scheduler.h` — Data structures and API.
- `Makefile` — For easy build/clean.
- `TestPlan.md` — Test cases and expected results.
- `output.log` — Sample output log.
- `screenshots/` — Place your screenshots here.

## Customization

Edit `main.c` to change module durations or number of GPUs.

## Author

*Your Name Here*