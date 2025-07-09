#include <SDL2/SDL.h>
#include <stdio.h>
#include "scheduler.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 400
#define GPU_WIDTH     200
#define GPU_HEIGHT    70
#define GPU_Y_START   80

// Task colors (RGB)
static int task_colors[NUM_TASKS][3] = {
    {70, 130, 180},   // Perception: SteelBlue
    {34, 139, 34},    // Localization: Green
    {255, 165, 0},    // Object Detection: Orange
    {255, 105, 180},  // Path Planning: Pink
    {128, 0, 128}     // Control: Purple
};

void draw_gpu(SDL_Renderer *renderer, int gpu_idx, int task_id, int progress, int total) {
    int x = 60 + gpu_idx * (GPU_WIDTH + 40);
    int y = GPU_Y_START;
    SDL_Rect gpu_rect = { x, y, GPU_WIDTH, GPU_HEIGHT };
    // Draw GPU box
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &gpu_rect);
    // Draw task progress bar if any
    if (task_id >= 0 && total > 0) {
        int bar_w = (progress * (GPU_WIDTH - 20)) / total;
        SDL_Rect bar = { x + 10, y + 30, bar_w, 20 };
        SDL_SetRenderDrawColor(renderer, task_colors[task_id][0], task_colors[task_id][1], task_colors[task_id][2], 255);
        SDL_RenderFillRect(renderer, &bar);
    }
    // Optionally: Add GPU label with SDL_ttf
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("GPU Scheduler Visualization (LPT)", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // --- Scheduler simulation state ---
    int module_durations[NUM_TASKS] = {70, 200, 190, 250, 300}; // ms
    Task tasks[NUM_TASKS];
    GPU gpus[NUM_GPUS];
    int total_work = 0, done = 0, time = 0;
    for (int i = 0; i < NUM_TASKS; ++i) {
        tasks[i].id = i;
        tasks[i].remaining = module_durations[i];
        tasks[i].total = module_durations[i];
        total_work += module_durations[i];
    }
    for (int i = 0; i < NUM_GPUS; ++i) {
        gpus[i].id = i;
        gpus[i].busy_until = 0;
        gpus[i].current_task = -1;
    }

    // --- Main loop ---
    int running = 1;
    SDL_Event event;
    Uint32 last_tick = SDL_GetTicks();

    while (running && done < NUM_TASKS) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }
        Uint32 now = SDL_GetTicks();
        if (now - last_tick >= 50) { // 50ms tick
            // Scheduler tick (almost identical to your scheduler.c)
            for (int g = 0; g < NUM_GPUS; ++g) {
                if (gpus[g].busy_until <= time) {
                    int chosen = select_task(tasks);
                    if (chosen >= 0) {
                        int chunk = (tasks[chosen].remaining >= 50) ? 50 : tasks[chosen].remaining;
                        tasks[chosen].remaining -= chunk;
                        gpus[g].busy_until = time + chunk;
                        gpus[g].current_task = chosen;
                        if (tasks[chosen].remaining == 0) {
                            done++;
                        }
                    } else {
                        gpus[g].current_task = -1;
                    }
                }
            }
            time += 50;
            last_tick = now;
        }

        // Draw GUI
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Draw GPUs and their current tasks/progress
        for (int g = 0; g < NUM_GPUS; ++g) {
            int t = gpus[g].current_task;
            int progress = 0, total = 1;
            if (t >= 0) {
                progress = tasks[t].total - tasks[t].remaining;
                total = tasks[t].total;
            }
            draw_gpu(renderer, g, t, progress, total);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}