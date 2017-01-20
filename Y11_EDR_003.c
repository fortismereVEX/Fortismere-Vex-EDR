#pragma config(Motor,  port1,           claw2,         tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           armL1,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           armL2,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           armR1,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           armR2,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           wheelLF,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           wheelLB,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           wheelRF,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           wheelRB,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          claw1,         tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

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

task autonomous()
{
	// ..........................................................................
	// Insert user code here.
	// ..........................................................................

	// Remove this function call once you have "real" code.
	AutonomousCodePlaceholderForTesting();
}

task usercontrol()
{
	// User control code here, inside the loop

	while(true)
	{
		// Stuff for the arms

		if (vexRT[Btn8U])
		{
			motor[armL1] = 127;
			motor[armL2] = 127;
			motor[armR1] = 127;
			motor[armR2] = 127;

		}
		else if (vexRT[Btn8D])
		{
			motor[armL1] = -127;
			motor[armL2] = -127;
			motor[armR1] = -127;
			motor[armR2] = -127;
		}
		else
		{
			motor[armL1] = 0;
			motor[armL2] = 0;
			motor[armR1] = 0;
			motor[armR2] = 0;
		}

		// fwd + strafe

		if (abs(vexRT[Ch4]) > abs(vexRT[Ch3]))
		{
			motor[wheelLF] = vexRT[Ch4];
			motor[wheelLB] = -vexRT[Ch4];

			motor[wheelRF] = vexRT[Ch4];
			motor[wheelRB] = -vexRT[Ch4];
		}
		else
		{
			motor[wheelLF] = vexRT[Ch3];
			motor[wheelLB] = vexRT[Ch3];

			motor[wheelRF] = -vexRT[Ch3];
			motor[wheelRB] = -vexRT[Ch3];
		}

		// turn
		if (vexRT[Ch1])
		{
			motor[wheelLF] = vexRT[Ch1];
			motor[wheelLB] = vexRT[Ch1];
			motor[wheelRF] = vexRT[Ch1];
			motor[wheelRB] = vexRT[Ch1];
		}

		if(vexRT(Btn6D))
		{
			motor[claw1] = 127;
			motor[claw2] = 127;
		}
		else if(vexRT(Btn6U))
		{
			motor[claw1] = -127;
			motor[claw2] = -127;
		}
		else
		{
			motor[claw1] = 0;
			motor[claw2] = 0;
		}
	}
}