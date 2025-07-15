#ifndef SCHEDULER_H
#define SCHEDULER_H

#define NUM_TASKS 5
#define NUM_GPUS  3

extern const char* TASK_NAMES[NUM_TASKS];

typedef struct {
    int id;
    int remaining;  // ms
    int total;      // ms
} Task;

typedef struct {
    int id;
    int busy_until; // time in ms
    int current_task; // task id, -1 if idle
} GPU;

void schedule(const int* task_durations);

#endif // SCHEDULER_H