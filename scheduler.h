#ifndef SCHEDULER_H
#define SCHEDULER_H

#define NUM_TASKS 5  // Number of tasks
#define NUM_GPUS  3 // Number of GPUs

extern const char* TASK_NAMES[NUM_TASKS];

// Structure to represent a task/module

typedef struct {
    int id;  //Task Id
    int remaining;  // Remaining time in ms
    int total;      // Total execution time in ms
} Task;

typedef struct {
    int id;  //GPU Id
    int busy_until; // Time (ms) until which GPU is busy
    int current_task; // task id currently running
} GPU;

// Run the LPT simulation
void schedule(int* task_durations);

#endif // SCHEDULER_H