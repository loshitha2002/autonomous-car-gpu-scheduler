#include "scheduler.h"

int main() {
    // Module durations in ms
    int module_durations[NUM_TASKS] = {70, 200, 190, 250, 300};

    schedule(module_durations);

    return 0;
}