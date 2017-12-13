#pragma once

#include "demo.hpp"
#include "lcd.hpp"
#include "main.hpp"

#include "motors.hpp"

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
#ifdef ROBOT_SAM
        auto inited = imeInitializeAll();
#else
        //int inited = 0;
#endif
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

        // @Hack for non pid travel
        power_right = (fwd - lr);
        power_left  = (fwd + lr);

        if ((fwd + lr) == 0) {
            // Tell pid that we no longer wish to move
            // reset to reset overall error that may have accumulated during travel
            pid_drive_left.reset();
            pid_drive_right.reset();
            pid_drive_left.set_requested(0);
            pid_drive_right.set_requested(0);
        } else {
            // ask pid to add this to our destination value
            pid_drive_left.set_requested(power_left);
            pid_drive_right.set_requested(power_right);
        }
    }

    static void run_intake() {
        // TODO: split into setting values here and
        // applying them in finalise to be consistent with other code
        if (get_joystick_digital(7, JOY_UP)) {
            motors::intake(127);
        } else if (get_joystick_digital(7, JOY_DOWN)) {
            motors::intake(-127);
        } else {
            motors::intake(0);
        }
    }

    static void run_lift() {
        if (get_joystick_digital(6, JOY_UP)) {
            motors::lift(127);
        } else if (get_joystick_digital(6, JOY_DOWN)) {
            motors::lift(-127);
        } else {
            motors::lift(0);
        }
    }

    static void run_arm() {
        if (get_joystick_digital(8, JOY_UP)) {
            motors::arm(-127);
        } else if (get_joystick_digital(8, JOY_DOWN)) {
            motors::arm(127);
        } else {
            motors::arm(0);
        }
    }

    static void run_claw() {
        if (get_joystick_digital(5, JOY_UP)) {
            motors::claw(127);
        } else if (get_joystick_digital(5, JOY_DOWN)) {

            motors::claw(-127);
        } else {
            motors::claw(0);
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
        motors::drive(power_left, power_right);
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
