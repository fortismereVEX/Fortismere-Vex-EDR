#pragma once

#include "demo.hpp"
#include "lcd.hpp"
#include "main.hpp"

#include "motors.hpp"

class drive {
    static bool inited;

    // are we replaying joystick / button commmands
    static bool in_demo;
    static bool in_recording;

    static int last_time;

    static bool slow_mode;

    // other state
    static bool claw_open;
    static int  claw_power;

#ifdef ROBOT_ROBBIE
    static bool arm_up;
#endif

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
        if (inited == true) return;

        // initialise imes
        imeShutdown();

        delay(1000);

        printf("==> imeInitializeAll()\n");
#ifdef ROBOT_SAM
        auto inited = imeInitializeAll();
#else
        auto inited = 0;
#endif
        printf("<=\n");

        lcd::printf("%d imes", inited);

// calibrate analogue sensors
#ifdef ROBOT_SAM
        analogCalibrate(8);
        // TODO: arm sensor
#elif defined(ROBOT_ROBBIE)
        // TODO: lift potentiometers
#endif
        inited = true;
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
#ifdef ROBOT_SAM
        if (get_joystick_digital(8, JOY_UP)) {
            motors::arm(-127);
        } else if (get_joystick_digital(8, JOY_DOWN)) {
            motors::arm(127);
        } else {
            motors::arm(0);
        }
#else
        if (get_joystick_digital(5, JOY_UP)) {
            arm_up = true;
            motors::arm(-127);
        } else if (get_joystick_digital(5, JOY_DOWN)) {
            arm_up = false;
            motors::arm(127);
        } else {
            motors::arm(0);
        }
#endif
    }

    static void run_claw() {
#ifdef ROBOT_SAM
        lcdPrint(uart1, 2, "%d", analogReadCalibrated(8));
        // -50 to 50 full open
        // -800 to -900 is full close

        if (get_joystick_digital(5, JOY_UP)) {
            // open
            claw_open  = true;
            claw_power = 127;
        } else if (get_joystick_digital(5, JOY_DOWN)) {
            // close

            claw_open  = false;
            claw_power = -127;
        } else {
            claw_power = 0;
        }
#elif defined(ROBOT_ROBBIE)
        if (get_joystick_digital(8, JOY_UP)) {
            claw_open = true; // roll up

            motors::claw(127);

        } else if (get_joystick_digital(8, JOY_DOWN)) {
            claw_open = false; // roll down
            motors::claw(-127);
        } else {
            // if (claw_open) {
            //     motors::claw(15);
            // } else {
            //     motors::claw(-15);
            // }
            motors::claw(0);
        }
#endif
    }

    static void pid_frame() {

        int dt = get_delta_time();

        pid_drive_left.set_dt(dt);
        pid_drive_right.set_dt(dt);

        pid_drive_left.step();
        pid_drive_right.step();

        int power_left  = pid_drive_left.get_power();
        int power_right = pid_drive_right.get_power();

        // printf("left: e: %.2f rea %.2f new %d\t\tright: e: %.2f rea %.2f new %d\n",
        //        pid_drive_left.get_error(), (float)pid_drive_left.get_tick(), power_left,
        //        pid_drive_right.get_error(), (float)pid_drive_right.get_tick(), power_right);

        //lcdPrint(uart1, 1, "e: %.2f", pid_drive_left.get_error());
        //lcdPrint(uart1, 2, "r: %.2f", pid_drive_left.get_tick());
        //lcdPrint(uart1, 1, "null? %s", !pid_drive_left.get_encoder().get_encoder() ? "true" : "false");
        //lcdPrint(uart1, 2, "v: %d", encoderGet(pid_drive_left.get_encoder().get_encoder()));

        if (abs(power_left) < 10) power_left = 0;
        if (abs(power_right) < 10) power_right = 0;

        power_left  = clamp((int)power_left, -127, 127);
        power_right = clamp((int)power_right, -127, 127);
    }

    static void finalise() {
        motors::drive(power_left, power_right);

        // finalise the claw values
#ifdef ROBOT_ROBBIE
        //motors::claw(claw_power);
#elif defined(ROBOT_SAM)
        // @TODO enum sensors
        int tick = analogReadCalibrated(8);
        if (claw_open == true) {
            if (tick > -100) {
                // we are essentially fully open so set power to a low amount
                motors::claw(0);
            } else {
                // attempt to open 4real
                motors::claw(127);
            }
        } else if (claw_open == false) {
            if (tick < -600) {
                // we missed the cone so just dont send any power (there is no point)
                motors::claw(0);
            } else if (tick < -300) {
                // we are closed around a cone so set to a low power to sustain cone hold
                motors::claw(-30);
            } else {
                // attempt to close 4real
                motors::claw(-127);
            }
        }
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
