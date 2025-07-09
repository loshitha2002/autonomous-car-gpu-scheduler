# Autonomous Car Task Scheduler (LPT)

## Description
This project simulates scheduling of key autonomous car modules (Perception, Localization, etc.) on three GPUs using the Longest Processing Time (LPT) heuristic in C.

You can view the results in the **console** or via a **graphical SDL visualization**.

---

## How to Compile and Run

### Console Version
```sh
gcc main.c scheduler.c -o scheduler
./scheduler
```

### SDL Visualization
**Requirements:** SDL2 development libraries  
On Ubuntu/Debian:  
```sh
sudo apt-get install libsdl2-dev
```
On Mac:  
```sh
brew install sdl2
```

**Build and Run:**
```sh
gcc scheduler_sdl.c scheduler.c -o scheduler_sdl -lSDL2
./scheduler_sdl
```

---

## Features
- Schedules 5 key autonomous car tasks on 3 GPUs (LPT heuristic)
- Console and graphical (SDL) versions
- Visualizes GPU progress and task assignments

## How to Modify Simulation
Edit the `module_durations` array in `main.c` or `scheduler_sdl.c`.

## Test Plan

See `TestPlan.md` for sample input/output checks.

## Sample Output

See `OutputLog.md`.

---