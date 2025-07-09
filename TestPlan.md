# Test Plan: Autonomous Car Task Scheduler

## Test Cases

| Test # | Input Durations             | Expected Output Summary                        |
|--------|-----------------------------|------------------------------------------------|
| 1      | {70, 200, 190, 250, 300}    | All tasks assigned and completed; finish time aligns with total work/3 or higher. |
| 2      | {100, 100, 100, 100, 100}   | All tasks distributed equally, minimal completion time. |
| 3      | {500, 50, 50, 50, 50}       | Longest task dominates schedule, LPT assigns it first. |
| 4      | {0, 0, 0, 0, 0}             | All tasks complete instantly.                  |

## Pass/Fail
- All tasks reported as completed.
- Final time matches expected results.
- No errors or crashes.