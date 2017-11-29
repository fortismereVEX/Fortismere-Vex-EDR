#include "main.hpp"

#include "lcd.hpp"
#include "util.hpp"

static pid_real_helper<ime> pid_drive_left(ime(0), 1.0f, 0.0f, 0.0f, 0.0f);
static pid_real_helper<ime> pid_drive_right(ime(1), 1.0f, 0.0f, 0.0f, 0.0f);

static float requested_right = 0.0f;
static float requested_left  = 0.0f;

static int last_time = 0;

static int get_delta_time() {
    int current_time = millis();
    int dt           = current_time - last_time;
    last_time        = current_time;
    return dt;
}

void pidtask(void *arg) {
    int dt = get_delta_time();

    pid_drive_left.set_dt(dt);
    pid_drive_left.set_requested(requested_left);

    pid_drive_right.set_dt(dt);
    pid_drive_right.set_requested(requested_right);

    pid_drive_left.step();
    pid_drive_right.step();

    char power_left  = pid_drive_left.get_new_power();
    char power_right = pid_drive_right.get_new_power();

    lcdPrint(uart1, 1, "e: %.2f r: %.2f", pid_drive_left.get_error(), requested_left);

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

void turn(int power) {
    motorSet(4, power);
    // back left
    motorSet(1, -power);

    motorSet(9, power);

    // front right
    motorSet(5, power);
    // back right
    motorSet(10, -power);

    motorSet(2, power);
}

void forward(char power) {
    // front left
    motorSet(4, power);
    // back left
    motorSet(1, -power);

    motorSet(9, power);

    // front right
    motorSet(5, -power);
    // back right
    motorSet(10, power);

    motorSet(2, -power);
}

void stop() {
    // front left
    motorSet(4, 0);
    // back left
    motorSet(1, -0);

    motorSet(9, 0);

    // front right
    motorSet(5, -0);
    // back right
    motorSet(10, 0);

    motorSet(2, -0);
}

void intake_down() {
    motorSet(6, -127);
    motorSet(3, 127);
}

void intake_up() {
    motorSet(6, 127);
    motorSet(3, -127);
}

void intake_stop() {
    motorSet(6, 0);
    motorSet(3, -0);
}

int g_autonomous = 0;

void autonomous() {
    TaskHandle pidtask_handle = taskCreate(&pidtask, TASK_DEFAULT_STACK_SIZE, nullptr, TASK_PRIORITY_DEFAULT);

    switch (g_autonomous) {
    case 0: {
        forward(127);

        intake_down();

        delay(800);

        intake_stop();

        delay(1100);

        stop();

        // pickup the mobo goal + turn

        intake_up();

        delay(500);

        intake_stop();

        turn(127);

        delay(1000);

        // go forwards and

        forward(127);
        delay(1700);

        intake_down();

        delay(500);

        forward(-127);

        delay(500);

        intake_up();

        delay(500);

        intake_stop();

        delay(500);

        stop();
        intake_stop();
        break;
    }
    case 1: {
        forward(127);
        delay(7000);
        intake_stop();
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
