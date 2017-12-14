#pragma once

#include "main.hpp"

#if defined(ROBOT_ROBBIE)
enum motor_ids {
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
enum motor_ids {
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

class motors {
public:
    motors() {}

    static void intake(char power) {
#ifdef ROBOT_SAM
        motorSet(motor_ids::mogo_left, power);
        motorSet(motor_ids::mogo_right, -power);
#else
        motorSet(motor_ids::mogo_lift, -power);
#endif
    }

    static void lift(char power) {
#ifdef ROBOT_SAM
        motorSet(motor_ids::lift, power);
#else
        motorSet(motor_ids::lift_left1, -power);
        motorSet(motor_ids::lift_left2, -power);

        motorSet(motor_ids::lift_right1, power);
        motorSet(motor_ids::lift_right2, power);
#endif
    }

    static void arm(char power) {
#ifdef ROBOT_SAM
        motorSet(motor_ids::arm_left, power);
        motorSet(motor_ids::arm_right, power);
#endif
    }

    static void claw(char power) {
#ifdef ROBOT_SAM
        motorSet(motor_ids::claw, power);
#else
        motorSet(claw_left, -power);
        motorSet(claw_right, power);
#endif
    }

    static void drive(char power_left, char power_right) {
#ifdef ROBOT_SAM
        motorSet(motor_ids::drive_left_back, power_left);
        motorSet(motor_ids::drive_left_middle, power_left);
        motorSet(motor_ids::drive_left_front, power_left);

        motorSet(motor_ids::drive_right_back, power_right);
        motorSet(motor_ids::drive_right_middle, power_right);
        motorSet(motor_ids::drive_right_front, power_right);
#elif defined(ROBOT_ROBBIE)
        motorSet(motor_ids::drive_left_middle, power_left);
        motorSet(motor_ids::drive_left_back, power_left);

        motorSet(motor_ids::drive_right_middle, power_right);
        motorSet(motor_ids::drive_right_back, power_right);
#endif
    }
};
