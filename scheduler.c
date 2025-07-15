#include "scheduler.h"
#include <stdio.h>

const char* TASK_NAMES[NUM_TASKS] = {
    "Perception (Sensor Fusion)",
    "Localization (Position Estimation)",
    "Object Detection",
    "Path Planning",
    "Control (Actuator Commands)"
};

static int select_task(Task* tasks) {
    int selected = -1;
    int max_remain = 0;
    for (int i = 0; i < NUM_TASKS; ++i) {
        if (tasks[i].remaining > max_remain) {
            max_remain = tasks[i].remaining;
            selected = i;
        }
    }
    return selected;
}

void schedule(const int* task_durations) {
    Task tasks[NUM_TASKS];
    GPU gpus[NUM_GPUS];
    int done = 0, time = 0, total_work = 0;

    for (int i = 0; i < NUM_TASKS; ++i) {
        tasks[i].id = i;
        tasks[i].remaining = task_durations[i];
        tasks[i].total = task_durations[i];
        total_work += task_durations[i];
    }
    for (int i = 0; i < NUM_GPUS; ++i) {
        gpus[i].id = i;
        gpus[i].busy_until = 0;
        gpus[i].current_task = -1;
    }

    printf("Autonomous Robot Car Packing Scheduler (LPT)\n");
    printf("GPUs: %d, Modules: %d\n", NUM_GPUS, NUM_TASKS);
    printf("Modules:\n");
    for (int i = 0; i < NUM_TASKS; ++i) {
        printf("  Task %d: %s (%d ms)\n", i, TASK_NAMES[i], tasks[i].total);
    }

    while (done < NUM_TASKS) {
        printf("\nTime: %d ms\n", time);
        for (int g = 0; g < NUM_GPUS; ++g) {
            if (gpus[g].busy_until <= time) {
                int chosen = select_task(tasks);
                if (chosen >= 0) {
                    int chunk = (tasks[chosen].remaining >= 50) ? 50 : tasks[chosen].remaining;
                    tasks[chosen].remaining -= chunk;
                    gpus[g].busy_until = time + chunk;
                    gpus[g].current_task = chosen;
                    printf("  GPU %d runs [%s] for %d ms (remaining: %d ms)\n",
                        g, TASK_NAMES[chosen], chunk, tasks[chosen].remaining);
                    if (tasks[chosen].remaining == 0) {
                        printf("    [%s] completed!\n", TASK_NAMES[chosen]);
                        done++;
                    }
                } else {
                    gpus[g].current_task = -1;
                }
            } else {
                int t = gpus[g].current_task;
                if (t >= 0)
                    printf("  GPU %d continues [%s] (busy until %d ms)\n",
                        g, TASK_NAMES[t], gpus[g].busy_until);
            }
        }
        time += 50;
    }

    printf("\nAll modules completed at %d ms\n", time);
    printf("Theoretical lower bound: %.2f ms\n", (float)total_work / NUM_GPUS);
}