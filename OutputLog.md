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

All modules completed at 550 ms
Theoretical lower bound: 202.00 ms
```