#pragma platform(VEX2)
#pragma competitionControl(Competition)

// Define for robbies robot and sams robot
#define ROBOT_ROBBIE
//#define ROBOT_SAM

#include "motor_config.h"

#include "robotc_base_include.c"

#include "user_control.c"
#include "auton_control.c"
#include "init.c"

// TODO: pid, (lcd), drive, auton selection, actual distance calibration

void init() {
    // Dont let robotc remove our tasks for us
    stop_tasks_between_mode = false;

    //taskStart(pid_task);
    //taskStart(lcd_task);


    // TODO: Select auton here
    //       Setup pid tasks
    //       Setup lcd task?

}
