#ifndef SCHEDULER_H
#define SCHEDULER_H

#define NUM_TASKS 5
#define NUM_GPUS  3

extern const char* TASK_NAMES[NUM_TASKS];

typedef struct {
    int id;
    int remaining;  // ms left
    int total;      // ms total
} Task;

typedef struct {
    int id;
    int busy_until; // time in ms when available
    int current_task; // task id, -1 if idle
} GPU;

int select_task(Task* tasks);
void schedule(int* task_durations);

#endif // SCHEDULER_H