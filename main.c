#include "scheduler.h"

int main() {
    int module_durations[NUM_TASKS] = {70, 200, 190, 250, 300}; // ms
    schedule(module_durations);
    return 0;
}