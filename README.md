# Autonomous Car GPU Scheduler (LPT Simulation)

## Overview

This project simulates a GPU scheduling algorithm for an autonomous vehicle, utilizing the **Longest Processing Time (LPT)** strategy. The scheduler assigns computation modules (tasks) to available GPUs in a way that aims to minimize the overall completion time (makespan). Each module represents a critical function of the autonomous car, such as perception, localization, and control.

The simulation provides clear console output showing scheduling decisions, task progress, and final statistics, making it useful for both academic demonstration and practical understanding of scheduling algorithms.

---

## Features

- **Modular Design:** Source code is split into logical files for maintainability (`main.c`, `scheduler.c`, `scheduler.h`).
- **Task Simulation:** Models 5 core autonomous car modules, each with customizable durations.
- **LPT Scheduling:** Assigns the most demanding (longest remaining) task to each available GPU at every scheduling tick.
- **Progress Tracking:** Console output details which GPU is running which task, remaining time, and completion notifications.
- **Performance Metrics:** Reports the makespan (actual completion time) and the theoretical optimal lower bound for comparison.
- **Easy Customization:** Modify task durations, number of GPUs, or simulation step size as needed.

---

## Autonomous Car Modules Simulated

1. **Perception (Sensor Fusion)**
2. **Localization (Position Estimation)**
3. **Object Detection**
4. **Path Planning**
5. **Control (Actuator Commands)**

---

## Folder Structure

```
autonomous-car-gpu-scheduler/
├── main.c
├── scheduler.c
├── scheduler.h
├── Makefile
├── README.md
├── TestPlan.md
├── output.log
└── screenshots/
```

---

## Build & Usage

### **Prerequisites**
- GCC or compatible C compiler
- Make utility (recommended for UNIX-like systems)

### **Build the Project**
```sh
make
```

### **Run the Simulation**
```sh
./scheduler
```

### **Clean Build Files**
```sh
make clean
```

---

## Customization

- **Task Durations:**  
  Edit the `module_durations` array in `main.c` to change the execution time (in milliseconds) of each module.

- **Number of GPUs/Tasks:**  
  Adjust `NUM_GPUS` and `NUM_TASKS` in `scheduler.h`.  
  Update related arrays in all source files accordingly.

---

## Output Example

A typical output will show module assignments, progress, and completion:

```
Autonomous Robot Car Packing Scheduler (LPT)
GPUs: 3, Modules: 5
Modules:
  Task 0: Perception (Sensor Fusion) (70 ms)
  Task 1: Localization (Position Estimation) (200 ms)
  Task 2: Object Detection (190 ms)
  Task 3: Path Planning (250 ms)
  Task 4: Control (Actuator Commands) (300 ms)

Time: 0 ms
  GPU 0 runs [Control (Actuator Commands)] for 50 ms (remaining: 250 ms)
  GPU 1 runs [Path Planning] for 50 ms (remaining: 200 ms)
  GPU 2 runs [Localization (Position Estimation)] for 50 ms (remaining: 150 ms)

...
All modules completed at 350 ms
Theoretical lower bound: 336.67 ms
```

For complete output, see [`output.log`](output.log).

---

## Test Plan

See [`TestPlan.md`](TestPlan.md) for detailed test scenarios, expected results, and validation procedures.

---

## Screenshots

Visual outputs are provided in the `screenshots/` directory.


---

## License

This project is provided for educational and demonstration purposes.

---
