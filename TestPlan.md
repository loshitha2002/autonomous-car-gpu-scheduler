# Test Plan: Autonomous Car GPU Scheduler

## Test Case 1: Default Input
- Input: Durations `{70, 200, 190, 250, 300}`, 3 GPUs
- Expected output: Each module scheduled once, makespan and theoretical lower bound reported.

## Test Case 2: Equal Durations
- Input: Durations `{100, 100, 100, 100, 100}`, 2 GPUs
- Expected output: All modules finish nearly together, correct lower bound.

## Test Case 3: One Large Task
- Input: Durations `{500, 50, 50, 50, 50}`, 2 GPUs
- Expected output: Large task dominates makespan.

## Test Case 4: More GPUs than Tasks
- Input: Durations `{50, 100}`, 4 GPUs
- Expected output: All modules assigned instantly, makespan = max duration.

For each test, verify output matches expected assignments and completion time.