void init();
task auton();
task user_control();

// This global controls whether this template stops tasks when the robot becomes
// disabled.
bool stop_tasks_between_mode = true;

// This global is used to disable the status display on the LCD by the code
// in this file.  The user may want to use the LCD during the pre_auton
// function and not have it overwritten by calls to displayStatusAndTime
bool display_status_on_lcd = true;

// There will also be many other errors but perhaps this will be noticed as it
// will be at the top of the list.  VEX platform is left for legacy reasons.
#if !defined(VEX2) && !defined(VEX)
#error "Switch to the VEX cortex platform"
#endif

void all_motors_off();
void all_tasks_stop();

/*---------------------------------------------------------------------------*/
task main() {
    // Master CPU will not let competition start until powered on for at least 2-seconds
    clearLCDLine(0);
    clearLCDLine(1);
    displayLCDPos(0, 0);
    displayNextLCDString("Startup");
    wait1Msec(2000);
    clearLCDLine(0);

    // Call pre_auton function where the user can initialize sensors and variables
    init();

    while (true) {
        // remain in this loop while the robot is disabled
        while (bIfiRobotDisabled) wait1Msec(25);

        if (bIfiAutonomousMode) {
            startTask(auton);

            // Waiting for autonomous phase to end
            while (bIfiAutonomousMode && !bIfiRobotDisabled) {
                if (!bVEXNETActive) {
                    // if the transmitters are powered off then disable the motors.
                    if (nVexRCReceiveState == vrNoXmiters) all_motors_off();
                }
                wait1Msec(25); // Waiting for autonomous phase to end
            }
            all_motors_off();
            if (stop_tasks_between_mode) all_tasks_stop();
        }

        else {
            // Start the usercontrol task
            startTask(user_control);

            // Here we repeat loop waiting for user control to end and (optionally) start
            // of a new competition run
            while (!bIfiAutonomousMode && !bIfiRobotDisabled) {
                if (nVexRCReceiveState == vrNoXmiters) all_motors_off();
                wait1Msec(25);
            }
            all_motors_off();
            if (stop_tasks_between_mode) all_tasks_stop();
        }
    }
}

// stop all the motors
void all_motors_off() {
    motor[port1] = 0;
    motor[port2] = 0;
    motor[port3] = 0;
    motor[port4] = 0;
    motor[port5] = 0;
    motor[port6] = 0;
    motor[port7] = 0;
    motor[port8] = 0;
#if defined(VEX2)
    motor[port9]  = 0;
    motor[port10] = 0;
#endif
}

// stop all the tasks
void all_tasks_stop() {
    stopTask(1);
    stopTask(2);
    stopTask(3);
    stopTask(4);
#if defined(VEX2)
    stopTask(5);
    stopTask(6);
    stopTask(7);
    stopTask(8);
    stopTask(9);
    stopTask(10);
    stopTask(11);
    stopTask(12);
    stopTask(13);
    stopTask(14);
    stopTask(15);
    stopTask(16);
    stopTask(17);
    stopTask(18);
    stopTask(19);
#endif
}
