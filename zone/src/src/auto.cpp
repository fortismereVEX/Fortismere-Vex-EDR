#include "main.hpp"

#include "lcd.hpp"
#include "util.hpp"

#include "drive.hpp"

#ifdef ROBOT_SAM
static pid_helper_real<ime> pid_drive_left(ime(0), 1.0f, 0.0f, 1.0f, 10.0f);
static pid_helper_real<ime> pid_drive_right(ime(1), 1.0f, 0.0f, 1.0f, 10.0f);
#endif

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

        motors::drive(power_left, power_right);

        delay(25);
    }
}

static bool at_dest() {
    if (pid_drive_left.at_dest(50) && pid_drive_right.at_dest(5 - 0)) {
        pid_drive_left.reset();
        pid_drive_right.reset();
        return true;
    }

    return false;
}

static void wait_for_dest() {
    while (!at_dest())
        delay(1);
}

// TODO: combine these with class drive to make this better

static void forward(float distance) {
    pid_drive_left.set_requested(distance);
    pid_drive_right.set_requested(-distance);
}

static void turn(float distance) {
    pid_drive_left.set_requested(distance);
    pid_drive_right.set_requested(distance);
}

// TODO: file reading here
int g_autonomous = 0;

static TaskHandle pidtask_handle = nullptr;
void              autonomous() {
    if (pidtask_handle == nullptr || taskGetState(pidtask_handle) == TASK_DEAD) {
        pidtask_handle = taskCreate(&pidtask, TASK_DEFAULT_STACK_SIZE, nullptr, TASK_PRIORITY_DEFAULT);
    }

    pid_drive_left.reset();
    pid_drive_right.reset();

    switch (g_autonomous) {
    case 0: {
        forward(1800);
        motors::arm(127);
        delay(500);
        motors::intake(127);
        delay(1600);
        motors::arm(0);
        motors::intake(0);
        wait_for_dest();
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

// TEMPORARY
enum class options {
    mogo_2c = 0,
    max     = mogo_2c,
};

static char *options_strings[] = {
    "mogo_2cone",
};

static void options_callback(options result) {
    extern int g_autonomous;
    g_autonomous = (int)result;
    lcd::printf("returned %d", g_autonomous);
}

void auto_initialize() {
    lcd::displayOptions(options::max, options_strings, &options_callback);
}
