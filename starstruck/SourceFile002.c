#pragma config(Motor,  port1,           motorLEFT,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           motorCENTRE,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           motorRARM,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           motorLARM,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          motorRIGHT,    tmotorVex393_HBridge, openLoop, reversed)
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
	// ..........................................................................
	// Insert user code here.
	// ..........................................................................
	sleep(3000);
	motor(motorLEFT) = 127;
	motor(motorRIGHT) = 127;
	motor(motorLARM) = 127;
	motor(motorRARM) = 127;
	sleep(3500);
	motor(motorLEFT) = -127;
	motor(motorRIGHT) = -127;
	motor(motorLARM) = -127;
	motor(motorRARM) = -127;
	sleep(2000);
	motor(motorLEFT) = 127;
	motor(motorRIGHT) = 127;
	motor(motorLARM) = 127;
	motor(motorRARM) = 127;
	sleep(2500);
	motor(motorLEFT) = -127;
	motor(motorRIGHT) = -127;
	motor(motorLARM) = -127;
	motor(motorRARM) = -127;
	sleep(2000);
	motor(motorLEFT) = 127;
	motor(motorRIGHT) = 127;
	motor(motorLARM) = 127;
	motor(motorRARM) = 127;
	// Remove this function call once you have "real" code.
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
	// User control code here, inside the loop

	while (true)
	{
		// This is the main execution loop for the user control program.
		// Each time through the loop your program should update motor + servo
		// values based on feedback from the joysticks.
		motor[motorLEFT] = vexRT[Ch2];
		motor[motorRIGHT] = vexRT[Ch3];
		motor[motorCENTRE] = vexRT[Ch1];
		motor[motorLARM] = vexRT[Ch2];
		motor[motorRARM] = vexRT[Ch3];
		if(vexRT[Btn7L]==1) {
				motor(motorLEFT) = 127;
				motor(motorRIGHT) = 127;
				motor(motorLARM) = 127;
				motor(motorRARM) = 127;
				sleep(3500);
				motor(motorLEFT) = -127;
				motor(motorRIGHT) = -127;
				motor(motorLARM) = -127;
				motor(motorRARM) = -127;
				sleep(2000);
				motor(motorLEFT) = 127;
				motor(motorRIGHT) = 127;
				motor(motorLARM) = 127;
				motor(motorRARM) = 127;
				sleep(2500);
				motor(motorLEFT) = -127;
				motor(motorRIGHT) = -127;
				motor(motorLARM) = -127;
				motor(motorRARM) = -127;
				sleep(2000);
				motor(motorLEFT) = 127;
				motor(motorRIGHT) = 127;
				motor(motorLARM) = 127;
				motor(motorRARM) = 127;
		}
		// ........................................................................
		// Insert user code here. This is where you use the joystick values to
		// update your motors, etc.
		// ........................................................................

		// Remove this function call once you have "real" code.
	}
}
