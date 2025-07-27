# Autonomous Car GPU Scheduler

## Overview

This repository implements a GPU scheduling simulator for autonomous vehicle modules using the **Longest Processing Time (LPT)** algorithm. The project includes:

- **fullcode.c**: The main scheduler logic and simulation (console-based).
- **test.c**: A user input interface for running custom scheduling scenarios via the terminal.
- **GUI.c**: A graphical interface for running the scheduler (requires additional setup; see below).

---

## Getting Started

### **Requirements**

- GCC or compatible C compiler
- [Optional for GUI] Simple graphics library (e.g., GTK, SDL, or similar; see GUI.c for details)
- Make utility (recommended)

---

## Usage Instructions

### **1. Compile the Programs**

Open your terminal in the project folder.

#### **A. Compile the main scheduler (fullcode.c):**

```sh
gcc fullcode.c -o scheduler
```

#### **B. Compile the user input test program (test.c):**

```sh
gcc test.c -o test
```

#### **C. Compile the GUI program (GUI.c):**

If your GUI uses a library such as GTK or SDL, you need to link against it. Example (for GTK):

```sh
gcc GUI.c -o gui `pkg-config --cflags --libs gtk+-3.0`
```

> **Note:** Adjust the compile command based on the graphics library used in `GUI.c`.  
> If you do not have a GUI library installed, you can skip this step and use the console programs.

---

### **2. Run the Programs**

#### **A. Run the main scheduler (console simulation):**

```sh
./scheduler
```

#### **B. Run the user input program:**

```sh
./test
```
You will be prompted to enter durations for each module, and the scheduler will output the schedule and makespan.

#### **C. Run the GUI program:**

```sh
./gui
```
> **Note:** GUI usage instructions may vary. Refer to comments in `GUI.c` for specific details.

---

## File Descriptions

- **fullcode.c** — The complete scheduler logic using LPT for autonomous car modules.
- **test.c** — Allows the user to input module durations interactively and view results in the terminal.
- **GUI.c** — Provides a graphical interface for entering durations and visualizing results (library dependencies apply).
- **Makefile** — Automates build steps (if present).
- **scheduler.h / scheduler.c** — Modularized code (if present; may be combined into one file in fullcode.c).
- **output.log** — Example output from simulations.
- **screenshots/** — Example screenshots from the GUI or console output.

---

## Example Usage

**Console User Input (`test.c`):**
```
Enter durations for each module (in ms):
  Perception (Sensor Fusion): 100
  Localization (Position Estimation): 250
  Object Detection: 120
  Path Planning: 200
  Control (Actuator Commands): 300

Autonomous Robot Car Packing Scheduler (LPT)
GPUs: 3, Modules: 5
...
All modules completed at 350 ms
Theoretical lower bound: 323.33 ms
```

---

## Customization

- **Change Module Durations:**  
  Edit values in `fullcode.c`, or use `test.c` for interactive input.
- **Change Number of GPUs or Modules:**  
  Adjust `NUM_GPUS` and `NUM_TASKS` in your source files.

---

## Troubleshooting

- If you get `gcc: command not found`, install GCC via your package manager.
- For GUI issues, ensure required libraries are installed and linked.
- If you encounter permission errors, run `chmod +x scheduler test gui` to make binaries executable.

---

## License

This project is released for educational and research purposes.

---

## Authors

- [Your Name Here]
- [Contributors]

---

## Screenshots

See the `screenshots/` directory for sample outputs.
