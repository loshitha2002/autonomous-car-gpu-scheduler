#include "scheduler.h"
#include <stdio.h>

// Task names representing Autonomous Car modules
const char* TASK_NAMES[NUM_TASKS] = {
    "Perception (Sensor Fusion)",
    "Localization (Position Estimation)",
    "Object Detection",
    "Path Planning",
    "Control (Actuator Commands)"
};

// Select the task with the most remaining time (LPT)
static int select_task(Task* tasks, int* busy_tasks) {
    int selected = -1;
    int max_remain = 0;
    for (int i = 0; i < NUM_TASKS; ++i) {
        // Find task with maximum remaining time
        if (tasks[i].remaining > max_remain && !busy_tasks[i]) {
            max_remain = tasks[i].remaining;
            selected = i;
        }
    }
    return selected;
}
// Main scheduling simulation function
void schedule(int* task_durations) {
    Task tasks[NUM_TASKS]; // Array on task
    GPU gpus[NUM_GPUS];   // Array on GPU
    int done = 0, time = 0, total_work = 0;

    // Initialize tasks and calculate total work
    for (int i = 0; i < NUM_TASKS; ++i) {
        tasks[i].id = i;
        tasks[i].remaining = task_durations[i];
        tasks[i].total = task_durations[i];
        total_work += task_durations[i];
    }
    //  Initialize GPUs
    for (int i = 0; i < NUM_GPUS; ++i) {
        gpus[i].id = i;
        gpus[i].busy_until = 0;
        gpus[i].current_task = -1;
    }
    // Print initial information
    printf("Autonomous Robot Car Packing Scheduler (LPT)\n");
    printf("GPUs: %d, Modules: %d\n", NUM_GPUS, NUM_TASKS);
    printf("Modules:\n");
    for (int i = 0; i < NUM_TASKS; ++i) {
        printf("  Task %d: %s (%d ms)\n", i, TASK_NAMES[i], tasks[i].total);
    }

    // Simulation loop
    while (done < NUM_TASKS) {
        printf("\nTime: %d ms\n", time);

        // Array to mark which tasks have been assigned in this time slot
        int busy_tasks[NUM_TASKS] = {0};

        // Mark currently running tasks as busy for this slot
        for (int g = 0; g < NUM_GPUS; ++g) {
            if (gpus[g].busy_until > time && gpus[g].current_task != -1) {
                busy_tasks[gpus[g].current_task] = 1;
            }
        }

        // Assign work to GPUs for this tick
        for (int g = 0; g < NUM_GPUS; ++g) {
            // If GPU is idle or becomes idle at this tick
            if (gpus[g].busy_until <= time) {
                int chosen = select_task(tasks, busy_tasks);
                if (chosen >= 0) {
                    // Assign up to 50 ms chunk or whatever remains
                    int chunk = (tasks[chosen].remaining >= 50) ? 50 : tasks[chosen].remaining;
                    tasks[chosen].remaining -= chunk;
                    gpus[g].busy_until = time + chunk;
                    gpus[g].current_task = chosen;
                    busy_tasks[chosen] = 1;
                    printf("  GPU %d runs [%s] for %d ms (remaining: %d ms)\n",
                        g, TASK_NAMES[chosen], chunk, tasks[chosen].remaining);
                    if (tasks[chosen].remaining == 0) {
                        printf("    [%s] completed!\n", TASK_NAMES[chosen]);
                        done++;
                    }
                } else {
                    // No task to assign
                    gpus[g].current_task = -1;
                }
            } else {
                // GPU is still working from previous tick
                int t = gpus[g].current_task;
                if (t >= 0)
                    printf("  GPU %d continues [%s] (busy until %d ms)\n",
                        g, TASK_NAMES[t], gpus[g].busy_until);
            }
        }
        time += 50;
    }
    // Print completion summary
    printf("\nAll modules completed at %d ms\n", time);
    printf("Theoretical lower bound: %.2f ms\n", (float)total_work / NUM_GPUS);
}