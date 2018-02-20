#include "main.hpp"

#include "lcd.hpp"
#include "util.hpp"

#include "drive.hpp"

#ifdef ROBOT_SAM
static pid_helper_real<ime> pid_drive_left(ime(0), 1.0f, 0.0f, 4.0f, 10.0f);
static pid_helper_real<ime> pid_drive_right(ime(1), 1.0f, 0.0f, 4.0f, 10.0f);
#else
static pid_helper_real<encoder> pid_drive_left(encoder(4, 5), 1.0f, 0.0f, 1.0f, 10.0f);
static pid_helper_real<encoder> pid_drive_right(encoder(4, 5), 1.0f, 0.0f, 1.0f, 10.0f);

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

        //lcdPrint(uart1, 2, "%.2f %.2f", (float)pid_drive_left.get_tick(), (float)pid_drive_left.get_requested());

        //lcdPrint(uart1, 1, "%.2f %.2f", (float)pid_drive_right.get_tick(), (float)pid_drive_right.get_requested());

        if (abs(power_left) < 10) power_left = 0;
        if (abs(power_right) < 10) power_right = 0;

        power_left  = clamp(power_left, -127, 127);
        power_right = clamp(power_right, -127, 127);

        lcdPrint(uart1, 1, "%d %d", power_left, power_right);
        lcdPrint(uart1, 2, "%.1f %.1f", pid_drive_left.get_error(), pid_drive_right.get_error());

        motors::drive(power_left, power_right);

        delay(5);
    }
}

static bool at_dest() {
    if (pid_drive_left.at_dest(50) && pid_drive_right.at_dest(50)) {
        pid_drive_left.reset();
        pid_drive_right.reset();
        return true;
    }

    return false;
}

static void wait_for_dest() {
    while (!at_dest())
        delay(21);
}

static void wait_for_dest(int timeout_ticks) {
    int start_time = millis();
    while (!at_dest() && start_time + timeout_ticks > millis()) delay(21);
}

    // TODO: combine these with class drive to make this better

#ifdef ROBOT_SAM
static void forward(float distance) {
    pid_drive_left.set_requested(distance);
    pid_drive_right.set_requested(-distance);
}
#else
static void forward(char power) {
    motors::drive(power, -power);
}
#endif
static void turn(float distance) {
#ifdef ROBOT_SAM
    pid_drive_left.set_requested(distance);
    pid_drive_right.set_requested(distance);
#else
    motors::drive(int(distance), int(distance));
#endif
}

extern bool init_inited;

// TODO: file reading here
int               g_autonomous   = 0;
static TaskHandle pidtask_handle = nullptr;

void autonomous() {
    if (init_inited == false) initialize();

    auto state = taskGetState(pidtask_handle);

    if (pidtask_handle == nullptr || state == TASK_DEAD) {
        pidtask_handle = taskCreate(&pidtask, TASK_DEFAULT_STACK_SIZE, nullptr, TASK_PRIORITY_DEFAULT);
    } else if (state == TASK_SUSPENDED) {
        taskResume(pidtask_handle);
    }

    pid_drive_left.reset();
    pid_drive_right.reset();

    delay(400);

    switch (g_autonomous) {
    case 0: {
#ifdef ROBOT_SAM
        // lift the arm up and the intake out ready to pick up the mobo goal
        motors::claw(-30);
        // start driving forward
        forward(1600);
        motors::arm(127);
        delay(500);
        motors::intake(-127);
        delay(600);
        motors::intake(0);
        delay(900);
        motors::arm(0);
        motors::intake(0);
        wait_for_dest();
        delay(100);

        // drive forwards and pickup the goal (stops it sliding out)
        forward(300);
        delay(100);

        // bring the arm back down onto the mobile goal
        motors::intake(127);
        motors::arm(-127);
        delay(800);
        motors::intake(0);
        delay(1500);
        motors::arm(0);
        wait_for_dest();

        // bring the arm back and over
        forward(-700);
        motors::claw(30);
        motors::arm(127);
        delay(300);
        motors::claw(-20);
        delay(800);
        motors::claw(0);
        delay(1300);
        motors::arm(0);
        wait_for_dest();

        // turn and bring the arm back up
        turn(750);
        wait_for_dest();

        // move forwards to the front of the zones
        forward(800);
        wait_for_dest();

        // turn slightly and head for the far mobile goal
        turn(500);
        //wait_for_dest();
        forward(1400);
        wait_for_dest(4000);

        // bring the intake out
        motors::intake(-127);
        delay(600);

        // drive backwards out of the zones
        forward(-1500);
        wait_for_dest();

#elif defined(ROBOT_ROBBIE)
        //TODO: these all use power values
        // make sure to change this when robbie attaches his sensors

        // reset the claw into the correct position
        motors::claw(127);
        delay(500);
        motors::claw(-127);
        delay(1000);

        // lift up the arm and drive forwards
        motors::lift(127);
        forward(127);
        delay(2000);

        // stop driving and stack the cone
        forward(0);
        motors::lift(-90);
        delay(1000);
        motors::lift(0);
#endif
        break;
    }
    case 1: {
#ifdef ROBOT_SAM
        forward(1500);
        wait_for_dest();
        break;
#else
        forward(127);
        delay(6000);
#endif
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

    if (pidtask_handle) {
        taskDelete(pidtask_handle);
        pidtask_handle = nullptr;
    }
}

// TEMPORARY
enum class options {
    mogo_2c = 0,
    forward = 1,
    off     = 2,
    max     = off,
};

static char *options_strings[] = {
    "mogo_2cone",
    "forward",
    "off",
};

static void options_callback(options result) {
    extern int g_autonomous;
    g_autonomous = (int)result;
    lcd::printf("returned %d", g_autonomous);
}

void auto_initialize() {
    pid_drive_left.reset();
    pid_drive_right.reset();

    lcd::displayOptions(options::max, options_strings, &options_callback);
}

void auto_finish() {
#ifdef ROBOT_SAM
    if (pidtask_handle) taskDelete(pidtask_handle);
    pidtask_handle = nullptr;
#endif
}
