# Test Plan: Autonomous Car GPU Scheduler

## Test Case 1: Default Input

- **Input:**  
  Module durations: `{70, 200, 190, 250, 300}` (ms)  
  GPUs: `3`
- **Expected Output:**  
  - Console output showing time steps, module assignments to GPUs, and remaining time.
  - Each module completes exactly once.
  - Total completion time ("All modules completed at ... ms") is printed and matches expected makespan.
  - Theoretical lower bound is printed:  
    `Theoretical lower bound: 202.00 ms`

## Test Case 2: All modules same length

- **Input:**  
  Module durations: `{100, 100, 100, 100, 100}`  
  GPUs: `2`
- **Expected Output:**  
  - Each GPU should be assigned a new module as soon as it becomes free.
  - All modules finish at the same time or one time step apart.
  - Theoretical lower bound matches: `250.00 ms` for total work `500/2`.

## Test Case 3: One module much longer

- **Input:**  
  Module durations: `{500, 50, 50, 50, 50}`  
  GPUs: `2`
- **Expected Output:**  
  - The long module dominates the makespan.
  - Theoretical lower bound: `700/2 = 350.00 ms`
  - Actual makespan should be `500 ms`, as the long task keeps one GPU busy the whole time.

## Test Case 4: More GPUs than tasks

- **Input:**  
  Module durations: `{50, 100}`  
  GPUs: `4`
- **Expected Output:**  
  - Each module gets a GPU instantly.
  - Makespan = longest module = `100 ms`.
  - Theoretical lower bound: `150/4 = 37.50 ms` (but actual = 100 ms).

---

**For each test:**  
- Check output matches expected GPU assignments and makespan.
- Record output log.
