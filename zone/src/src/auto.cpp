#include "main.hpp"

void pidtask(void *arg) {
}

int g_autonomous = -1;

void autonomous() {
    TaskHandle pidtask_handle = taskCreate(&pidtask, TASK_DEFAULT_STACK_SIZE, nullptr, TASK_PRIORITY_DEFAULT);

    switch (g_autonomous) {
    case 0: {
        break;
    }
    case 1: {
        break;
    }
    case 2: {
        break;
    }
    case 3: {
        break;
    }
    default: {
        // whoops you didnt select one
        // dont make any assumptions here
    }
    }
}
