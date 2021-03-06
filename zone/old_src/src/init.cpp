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

extern "C" {
  void __libc_init_array();
}


/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */

 bool g_Ready;


void initializeIO() {
	g_Ready = false;
}


void initialize() {
	__libc_init_array();

	EncoderLeft = AllocateEncoderSetup(2);
	EncoderLeft->encoder = encoderInit(Sensors::LeftQuadTop, Sensors::LeftQuadBot, false);
	EncoderLeft->motors[0] = Motors::DriveLeft1;
	EncoderLeft->motors[1] = Motors::DriveLeft2;

	EncoderLeft->requestedValue = 0.0f;
	EncoderLeft->constant_p = 20.0f;
	EncoderLeft->constant_i = 0.0;
	EncoderLeft->constant_d = 2.0f;

	EncoderRight = AllocateEncoderSetup(2);
	EncoderRight->encoder = encoderInit(Sensors::RightQuadTop, Sensors::RightQuadBot, false);
	EncoderRight->motors[0] = Motors::DriveRight1;
	EncoderRight->motors[1] = Motors::DriveRight2;

	EncoderRight->requestedValue = 0.0f;
	EncoderRight->constant_p = 20.0f;
	EncoderRight->constant_i = 0.0;
	EncoderRight->constant_d = 2.0;

	//taskCreate(&pidTask, TASK_DEFAULT_STACK_SIZE, EncoderRight, TASK_PRIORITY_DEFAULT);
	//taskCreate(&pidTask, TASK_DEFAULT_STACK_SIZE, EncoderLeft, TASK_PRIORITY_DEFAULT);

	//delay(5000);

	Log::Print("Initialisation complete\n");

	// start the menu task
	taskCreate(&LCD::LcdTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

	g_Ready = true;

}
