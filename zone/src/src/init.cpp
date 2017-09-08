/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "lcdmenu.h"
#include "portSetup.h"
#include "pid.h"

// TODO: MOVE THESE
EncoderSetup *EncoderRight;
EncoderSetup *EncoderLeft;

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO() {
}

/*
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An autonomous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */
void initialize() {

	// initialise the lcd
	lcdInit(uart1);
	lcdClear(uart1);
	lcdSetBacklight(uart1, true);

	EncoderLeft = AllocateEncoderSetup(2);
	EncoderLeft->encoder = encoderInit(Sensors::LeftQuadTop, Sensors::LeftQuadBot, false);
	EncoderLeft->motors[0] = Motors::DriveLeft1;
	EncoderLeft->motors[1] = Motors::DriveLeft2;

	EncoderLeft->requestedValue = 0.0f;
	EncoderLeft->constant_p = 2.0;
	EncoderLeft->constant_i = 0.04;
	EncoderLeft->constant_d = 0.1;

	EncoderRight = AllocateEncoderSetup(2);
	EncoderRight->encoder = encoderInit(Sensors::RightQuadTop, Sensors::RightQuadBot, false);
	EncoderRight->motors[0] = Motors::DriveRight1;
	EncoderRight->motors[1] = Motors::DriveRight2;

	EncoderRight->requestedValue = 0.0f;
	EncoderRight->constant_p = 2.0;
	EncoderRight->constant_i = 0.04;
	EncoderRight->constant_d = 0.1;

	taskCreate(&pidTask, TASK_DEFAULT_STACK_SIZE, EncoderRight, TASK_PRIORITY_DEFAULT);
	taskCreate(&pidTask, TASK_DEFAULT_STACK_SIZE, EncoderLeft, TASK_PRIORITY_DEFAULT);

	// start the menu task
	taskCreate(&LCD::LcdTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
}
