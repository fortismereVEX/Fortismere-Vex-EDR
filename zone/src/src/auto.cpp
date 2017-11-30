#include "main.hpp"

#include "lcd.hpp"
#include "util.hpp"

static pid_helper_real<ime> pid_drive_left(ime(0), 0.9f, 0.2f, 5.0f, 10.0f);
static pid_helper_real<ime> pid_drive_right(ime(1), 0.9f, 0.2f, 5.0f, 10.0f);

static int last_time = 0;

static int get_delta_time() {
    int current_time = millis();
    int dt           = current_time - last_time;
    last_time        = current_time;
    return dt;
}

void pidtask(void *arg) {

    while (true) {
        int dt = get_delta_time();

        pid_drive_left.set_dt(dt);
        pid_drive_right.set_dt(dt);

        pid_drive_left.step();
        pid_drive_right.step();

        int power_left  = pid_drive_left.get_power();
        int power_right = pid_drive_right.get_power();

        printf("left: e: %.2f rea %.2f new %d\t\tright: e: %.2f rea %.2f new %d\n",
               pid_drive_left.get_error(), (float)pid_drive_left.get_tick(), power_left,
               pid_drive_right.get_error(), (float)pid_drive_right.get_tick(), power_right);

        if (abs(power_left) < 10) power_left = 0;
        if (abs(power_right) < 10) power_right = 0;

        power_left  = clamp(power_left, -128, 128);
        power_right = clamp(power_right, -128, 128);

        // front left
        motorSet(4, power_left);
        // back left
        motorSet(1, -power_left);

        motorSet(9, power_left);

        // front right
        motorSet(5, power_right);
        // back right
        motorSet(10, -power_right);

        motorSet(2, power_right);

        delay(25);
    }
}

void forward(float distance) {
    pid_drive_left.set_requested(distance);
    pid_drive_right.set_requested(-distance);
}

void turn(float distance) {
    pid_drive_left.set_requested(distance);
    pid_drive_right.set_requested(distance);
}

int g_autonomous = 0;

static TaskHandle pidtask_handle = nullptr;

void autonomous() {
    if (pidtask_handle == nullptr || taskGetState(pidtask_handle) == TASK_DEAD) {
        pidtask_handle = taskCreate(&pidtask, TASK_DEFAULT_STACK_SIZE, nullptr, TASK_PRIORITY_DEFAULT);
    }

    pid_drive_left.reset();
    pid_drive_right.reset();

    switch (g_autonomous) {
    case 0: {
        forward(300);
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
