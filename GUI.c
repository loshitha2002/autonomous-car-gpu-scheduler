#include <gtk/gtk.h>
#include <stdio.h>
#include <stdarg.h>

#define NUM_TASKS 5
#define NUM_GPUS  3
#define TICK_MS   50

const char *TASK_NAMES[NUM_TASKS] = {
    "Perception (Sensor Fusion)",
    "Localization (Position Estimation)",
    "Object Detection",
    "Path Planning",
    "Control (Actuator Commands)"
};

typedef struct {
    int id;
    int remaining;
    int total;
} Task;

typedef struct {
    int busy_until;
    int current_task;
} GPUState;

GtkWidget *spin_buttons[NUM_TASKS];
GtkTextBuffer *gpu_buffers[NUM_GPUS];
GtkTextBuffer *summary_buffer;
Task tasks[NUM_TASKS];
GPUState gpus[NUM_GPUS];
int total_work = 0, done = 0, completed = 0, current_time = 0;

void append_to_buffer(GtkTextBuffer *buffer, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char line[512];
    vsnprintf(line, sizeof(line), fmt, args);
    va_end(args);

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, line, -1);
}

int select_lpt_task(int *busy_flags) {
    int max = 0, selected = -1;
    for (int i = 0; i < NUM_TASKS; i++) {
        if (tasks[i].remaining > max && !busy_flags[i]) {
            max = tasks[i].remaining;
            selected = i;
        }
    }
    return selected;
}

void run_simulation(GtkWidget *widget, gpointer user_data) {
    done = 0;
    completed = 0;
    current_time = 0;
    total_work = 0;

    for (int i = 0; i < NUM_TASKS; i++) {
        tasks[i].id = i;
        tasks[i].remaining = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_buttons[i]));
        tasks[i].total = tasks[i].remaining;
        total_work += tasks[i].remaining;
    }

    for (int g = 0; g < NUM_GPUS; g++) {
        gpus[g].busy_until = 0;
        gpus[g].current_task = -1;
        gtk_text_buffer_set_text(gpu_buffers[g], "", -1);
    }
    gtk_text_buffer_set_text(summary_buffer, "", -1);

    append_to_buffer(summary_buffer, "\xf0\x9f\x9a\x80 Simulation Start\n\n");

    while (done < NUM_TASKS) {
        int busy_flags[NUM_TASKS] = {0};

        for (int g = 0; g < NUM_GPUS; g++) {
            if (gpus[g].busy_until > current_time && gpus[g].current_task != -1)
                busy_flags[gpus[g].current_task] = 1;
        }

        for (int g = 0; g < NUM_GPUS; g++) {
            if (gpus[g].busy_until <= current_time) {
                int chosen = select_lpt_task(busy_flags);
                if (chosen >= 0) {
                    int chunk = (tasks[chosen].remaining >= TICK_MS) ? TICK_MS : tasks[chosen].remaining;
                    tasks[chosen].remaining -= chunk;
                    gpus[g].busy_until = current_time + chunk;
                    gpus[g].current_task = chosen;
                    busy_flags[chosen] = 1;

                    append_to_buffer(gpu_buffers[g],
                        "Time %3dms: Run [%s] for %dms (Remaining: %dms)\n",
                        current_time, TASK_NAMES[chosen], chunk, tasks[chosen].remaining);

                    if (tasks[chosen].remaining == 0) {
                        append_to_buffer(gpu_buffers[g], "           \xe2\x9c\x85 [%s] done!\n", TASK_NAMES[chosen]);
                        done++;
                    }
                } else {
                    gpus[g].current_task = -1;
                }
            } else {
                int t = gpus[g].current_task;
                if (t >= 0) {
                    append_to_buffer(gpu_buffers[g],
                        "Time %3dms: Continuing [%s] (Until %dms)\n",
                        current_time, TASK_NAMES[t], gpus[g].busy_until);
                }
            }
        }

        current_time += TICK_MS;
        while (g_main_context_iteration(NULL, FALSE));
    }

    append_to_buffer(summary_buffer, "\n\xf0\x9f\x8e\x89 All tasks completed at %dms\n", current_time);
    append_to_buffer(summary_buffer, "\xf0\x9f\x93\x8f Theoretical min time: %.2fms\n", (float)total_work / NUM_GPUS);
}

GtkWidget* create_log_box(const char *title, GtkTextBuffer **buf_store) {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);

    GtkWidget *label = gtk_label_new(title);
    gtk_widget_set_name(label, "gpu-label");
    gtk_box_append(GTK_BOX(vbox), label);

    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_box_append(GTK_BOX(vbox), scrolled);

    GtkWidget *textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(textview), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), textview);

    *buf_store = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

    gtk_widget_add_css_class(vbox, "log-frame");

    return vbox;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GPU Scheduler - GTK4");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);

    GtkWidget *overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(window), overlay);

    GFile *file = g_file_new_for_path("background.jpg");
    GtkWidget *bg = gtk_picture_new_for_file(file);
    gtk_picture_set_content_fit(GTK_PICTURE(bg), GTK_CONTENT_FIT_FILL);
    gtk_widget_set_hexpand(bg, TRUE);
    gtk_widget_set_vexpand(bg, TRUE);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), bg);

    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(main_vbox, 20);
    gtk_widget_set_margin_bottom(main_vbox, 20);
    gtk_widget_set_margin_start(main_vbox, 20);
    gtk_widget_set_margin_end(main_vbox, 20);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), main_vbox);

    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css,
        "* { font-size: 18px; }"
        "#gpu-label { color: white; font-weight: bold; font-size: 18px; }"
        ".task-label { color: white; font-weight: bold; font-size: 18px; }"
        ".log-frame { background-color: rgba(0,0,0,0.4); border-radius: 10px; padding: 8px; }",
        -1);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *center_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(center_box, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(main_vbox), center_box);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 30);
    gtk_widget_set_halign(grid, GTK_ALIGN_FILL);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(center_box), grid);

    for (int i = 0; i < NUM_TASKS; i++) {
        GtkWidget *label = gtk_label_new(TASK_NAMES[i]);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_widget_add_css_class(label, "task-label");

        spin_buttons[i] = gtk_spin_button_new_with_range(50, 1000, 10);
        gtk_widget_set_size_request(spin_buttons[i], 100, -1);
        gtk_widget_set_halign(spin_buttons[i], GTK_ALIGN_END);

        gtk_grid_attach(GTK_GRID(grid), label, 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), spin_buttons[i], 1, i, 1, 1);
    }

    GtkWidget *run_btn = gtk_button_new_with_label("▶ Run Simulation");
    gtk_widget_set_halign(run_btn, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(main_vbox), run_btn);
    g_signal_connect(run_btn, "clicked", G_CALLBACK(run_simulation), NULL);

    GtkWidget *gpu_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_hexpand(gpu_row, TRUE);
    gtk_box_append(GTK_BOX(main_vbox), gpu_row);

    for (int i = 0; i < NUM_GPUS; i++) {
        GtkWidget *log_box = create_log_box(i == 0 ? "🟦 GPU 1" : (i == 1 ? "🟩 GPU 2" : "🟥 GPU 3"), &gpu_buffers[i]);
        gtk_widget_set_hexpand(log_box, TRUE);
        gtk_box_append(GTK_BOX(gpu_row), log_box);
    }

    GtkWidget *summary_frame = create_log_box("📊 Summary", &summary_buffer);
    gtk_widget_set_vexpand(summary_frame, TRUE);
    gtk_box_append(GTK_BOX(main_vbox), summary_frame);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.GpuScheduler", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}
