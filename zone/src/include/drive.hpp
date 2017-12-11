#pragma once

#include "demo.hpp"
#include "lcd.hpp"
#include "main.hpp"

#if defined(ROBOT_ROBBIE)
enum motors {
    mogo_lift = 1,

    lift_left1 = 3,
    lift_left2 = 6,

    lift_right1 = 8,
    lift_right2 = 7,

    claw_left  = 2,
    claw_right = 9,

    drive_left_middle = 4,
    drive_left_back   = 4,

    drive_right_middle = 5,
    drive_right_back   = 5,

};
#elif defined(ROBOT_SAM)
enum motors {
    arm_left  = 1,
    arm_right = 10,

    drive_left_front  = 3,
    drive_left_middle = 7,
    drive_left_back   = 7,

    drive_right_front  = 9,
    drive_right_middle = 5,
    drive_right_back   = 5,

    mogo_right = 2,
    mogo_left  = 4,

    claw = 6,

    lift = 8,
};
#endif

class drive {
    // are we replaying joystick / button commmands
    static bool in_demo;
    static bool in_recording;

    static int last_time;

    static bool slow_mode;

#ifdef ROBOT_SAM
    static pid_helper_real<ime> pid_drive_left;
    static pid_helper_real<ime> pid_drive_right;
#else
    static pid_helper_real<encoder> pid_drive_left;
    static pid_helper_real<encoder> pid_drive_right;
#endif

    static char  power_left;
    static char  power_right;
    static float requested_left;
    static float requested_right;

    static int get_delta_time() {
        int current_time = millis();
        int dt           = current_time - last_time;
        last_time        = current_time;
        return dt;
    }

public:
    static void initialize() {

        imeShutdown();

        delay(1000);

        printf("==> imeInitializeAll()\n");
        //auto inited = imeInitializeAll();
        int inited = 0;

        printf("<=\n");

        lcd::printf("%d imes", inited);
    }

    static int get_joystick_analog(int axis) {
        if (in_demo == true) {
            // return next byte
            return 0;
        }

        char value = joystickGetAnalog(1, axis);

        // account for dead space
        // TODO: we shouldnt need to deal with this
        if (abs(value) < 20) value = 0;

        if (in_recording == true) {
            // write to buffer
        }

        return value;
    }
    static bool get_joystick_digital(int group, int direction) {
        if (in_demo == true) {
            // return next byte
            return 0;
        } else if (in_recording == true) {
            char value = joystickGetDigital(1, group, direction);
            // store value in buffer
            return value;
        }
        return joystickGetDigital(1, group, direction);
    }

    static void run_drive() {
        int fwd = get_joystick_analog(1);
        int lr  = get_joystick_analog(3);

        power_right = (fwd - lr);
        power_left  = (fwd + lr);

        if ((fwd + lr) == 0) {
            pid_drive_left.reset();
            pid_drive_right.reset();

            pid_drive_left.set_requested(power_left);
            pid_drive_right.set_requested(power_right);
        } else {
            pid_drive_left.set_requested(power_left);
            pid_drive_right.set_requested(power_right);
        }
    }

    static void run_intake() {
        // TODO: split into setting values here and
        // applying them in finalise to be consistent with other code
        if (get_joystick_digital(7, JOY_UP)) {
#ifdef ROBOT_SAM
            motorSet(motors::mogo_left, -127);
            motorSet(motors::mogo_right, 127);
#else
            motorSet(motors::mogo_lift, -127);
#endif
        } else if (get_joystick_digital(7, JOY_DOWN)) {
#ifdef ROBOT_SAM
            motorSet(motors::mogo_left, 127);
            motorSet(motors::mogo_right, -127);
#else
            motorSet(motors::mogo_lift, 127);
#endif
        } else {
#ifdef ROBOT_SAM
            motorSet(motors::mogo_left, 0);
            motorSet(motors::mogo_right, 0);
#else
            motorSet(motors::mogo_lift, 0);
#endif
        }
    }

    static void run_lift() {
        if (get_joystick_digital(6, JOY_UP)) {
#ifdef ROBOT_SAM
            motorSet(motors::lift, 127);
#else
            motorSet(motors::lift_left1, -127);
            motorSet(motors::lift_left2, -127);

            motorSet(motors::lift_right1, 127);
            motorSet(motors::lift_right2, 127);
#endif
        } else if (get_joystick_digital(6, JOY_DOWN)) {
#ifdef ROBOT_SAM
            motorSet(motors::lift, -127);
#else
            motorSet(motors::lift_left1, 127);
            motorSet(motors::lift_left2, 127);

            motorSet(motors::lift_right1, -127);
            motorSet(motors::lift_right2, -127);
#endif
        } else {
#ifdef ROBOT_SAM
            motorSet(motors::lift, 0);
#else
            motorSet(motors::lift_left1, 0);
            motorSet(motors::lift_left2, 0);

            motorSet(motors::lift_right1, 0);
            motorSet(motors::lift_right2, 0);
#endif
        }
    }

    static void run_arm() {
#ifdef ROBOT_SAM
        if (get_joystick_digital(8, JOY_UP)) {
            motorSet(motors::arm_left, -127);
            motorSet(motors::arm_right, -127);
        } else if (get_joystick_digital(8, JOY_DOWN)) {
            motorSet(motors::arm_left, 127);
            motorSet(motors::arm_right, 127);
        } else {
            motorSet(motors::arm_left, 0);
            motorSet(motors::arm_right, 0);
        }
#endif
    }

    static void run_claw() {
        if (get_joystick_digital(5, JOY_UP)) {
#ifdef ROBOT_SAM
            motorSet(motors::claw, 127);
#else
            motorSet(claw_left, -127);
            motorSet(claw_right, 127);
#endif
        } else if (get_joystick_digital(5, JOY_DOWN)) {
#ifdef ROBOT_SAM
            motorSet(motors::claw, -127);
#else
            motorSet(claw_left, 127);
            motorSet(claw_right, -127);
#endif
        } else {
#ifdef ROBOT_SAM
            motorSet(motors::claw, 0);
#else
            motorSet(claw_left, 0);
            motorSet(claw_right, 0);
#endif
        }
    }

    static void pid_frame() {
        if (get_joystick_digital(8, JOY_UP))
            slow_mode = true;
        else if (get_joystick_digital(8, JOY_DOWN)) {
            slow_mode = false;
        }
        if (slow_mode == true) {
            power_left  = (float)power_left / 1.5;
            power_right = (float)power_right / 1.5;
        }

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

        power_left  = clamp((int)power_left, -128, 128);
        power_right = clamp((int)power_right, -128, 128);
    }

    static void finalise() {
// TODO: merge sam and robbies motor code when i add the enum
// for it
#ifdef ROBOT_SAM
        motorSet(motors::drive_left_back, power_left);
        motorSet(motors::drive_left_middle, power_left);
        motorSet(motors::drive_left_front, power_left);

        motorSet(motors::drive_right_back, power_right);
        motorSet(motors::drive_right_middle, power_right);
        motorSet(motors::drive_right_front, power_right);
#elif defined(ROBOT_ROBBIE)
        motorSet(motors::drive_left_middle, power_left);
        motorSet(motors::drive_left_back, power_left);

        motorSet(motors::drive_right_middle, power_right);
        motorSet(motors::drive_right_back, power_right);
#endif
    }

    static void run_frame() {
        // build motor values
        run_drive();
        run_intake();
        run_lift();
        run_arm();
        run_claw();

        // run pid where needed
        pid_frame();

        // set final motor values
        finalise();
    }
};
