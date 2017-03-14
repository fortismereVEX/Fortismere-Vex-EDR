#pragma config(Sensor, dgtl1,  ArmEncoder,     sensorQuadEncoder)
#pragma config(Motor,  port1,           ClawL,         tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           LiftLD,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           LiftRD,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           DriveL1,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           DriveL2,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           DriveL3,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           DriveR1,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           DriveR2,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           DriveR3,       tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          ClawR,         tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// autonomous helpers
void ZeroDrive()
{
	int power = 0;
	// Left
	motor[DriveL1] = power;
	motor[DriveL2] = power;
	motor[DriveL3] = power;

	//Right
	motor[DriveR1] = power;
	motor[DriveR2] = power;
	motor[DriveR3] = power;
}

void ZeroClaw()
{
	int power = 0;

	motor[ClawL] = power;
	motor[ClawR] = power;
}

void ZeroArm()
{
	int power = 0;

	motor[LiftLD] = power;
	motor[LiftRD] = power;
}

void MoveForward(int time)
{
	int power = 127;
	// Left
	motor[DriveL1] = power;
	motor[DriveL2] = power;
	motor[DriveL3] = power;

	//Right
	motor[DriveR1] = power;
	motor[DriveR2] = power;
	motor[DriveR3] = power;

	sleep(time);

	ZeroDrive();
}

void MoveForward()
{
	int power = 127;
	// Left
	motor[DriveL1] = power;
	motor[DriveL2] = power;
	motor[DriveL3] = power;

	//Right
	motor[DriveR1] = power;
	motor[DriveR2] = power;
	motor[DriveR3] = power;
}

void MoveBackwards(int time)
{
	int power = 127;
	// Left
	motor[DriveL1] = -power;
	motor[DriveL2] = -power;
	motor[DriveL3] = -power;

	//Right
	motor[DriveR1] = -power;
	motor[DriveR2] = -power;
	motor[DriveR3] = -power;

	sleep(time);

	ZeroDrive();
}

void MoveBackwards()
{
	int power = 127;
	// Left
	motor[DriveL1] = -power;
	motor[DriveL2] = -power;
	motor[DriveL3] = -power;

	//Right
	motor[DriveR1] = -power;
	motor[DriveR2] = -power;
	motor[DriveR3] = -power;
}

void TurnLeft(int time)
{
	int power = 127;
	// Left
	motor[DriveL1] = -power;
	motor[DriveL2] = -power;
	motor[DriveL3] = -power;

	//Right
	motor[DriveR1] = power;
	motor[DriveR2] = power;
	motor[DriveR3] = power;

	sleep(time);

	ZeroDrive();
}

void TurnLeft()
{
	int power = 127;
	// Left
	motor[DriveL1] = -power;
	motor[DriveL2] = -power;
	motor[DriveL3] = -power;

	//Right
	motor[DriveR1] = power;
	motor[DriveR2] = power;
	motor[DriveR3] = power;
}

void TurnRight(int time)
{
	int power = 127;
	// Left
	motor[DriveL1] = power;
	motor[DriveL2] = power;
	motor[DriveL3] = power;

	//Right
	motor[DriveR1] = -power;
	motor[DriveR2] = -power;
	motor[DriveR3] = -power;

	sleep(time);

	ZeroDrive();
}

void TurnRight()
{
	int power = 127;
	// Left
	motor[DriveL1] = power;
	motor[DriveL2] = power;
	motor[DriveL3] = power;

	//Right
	motor[DriveR1] = -power;
	motor[DriveR2] = -power;
	motor[DriveR3] = -power;
}

void ClawOpen(int time)
{
	int power = 127;

	motor[ClawL] = power;
	motor[ClawR] = power;

	sleep(time);

	ZeroClaw();
}

void ClawOpen()
{
	int power = 127;

	motor[ClawL] = power;
	motor[ClawR] = power;
}

void ClawClose(int time)
{
	int power = 127;

	motor[ClawL] = -power;
	motor[ClawR] = -power;

	sleep(time);

	ZeroClaw();
}

void ClawClose()
{
	int power = 127;

	motor[ClawL] = power;
	motor[ClawR] = power;
}

void ArmUp(int time)
{
	int power = 127;

	motor[ClawL] = power;
	motor[ClawR] = power;

	sleep(time);
	ZeroArm();
}

void ArmUp()
{
	int power = 127;

	motor[ClawL] = power;
	motor[ClawR] = power;
}

void ArmDown(int time)
{
	int power = 127;

	motor[ClawL] = -power;
	motor[ClawR] = -power;

	sleep(time);
	ZeroArm();
}

void ArmDown()
{
	int power = 127;

	motor[ClawL] = -power;
	motor[ClawR] = -power;
}

void AutonomousCubeRight()
{
	//motor[ClawL] = 127;
	//motor[ClawR] = 127;
  ClawOpen();
	sleep(600);
	//motor[ClawL] = 0;
	//motor[ClawR] = 0;
	ZeroClaw();
	sleep(50);
	//motor[ClawL] = -127;
	//motor[ClawR] = -127;
	ClawClose();
	sleep(154);
	//motor[ClawL] = 0;
	//motor[ClawR] = 0;
	ZeroClaw();
	//motor[LiftLD] = -127;
	//motor[LiftRD] = -127;
	ArmDown();
	sleep(200);
	//motor[LiftRD] = 0;
	//motor[LiftRD] = 0;
	ZeroArm();
	MoveForward(850);
	//motor[ClawL] = 127;
	//motor[ClawR] = 127;
	ClawOpen();
	sleep(600);

	//turning
	//motor[DriveL1] = -127;
	//motor[DriveL2] = -127;
	//motor[DriveL3] = -127;
	//motor[DriveR1] = 127;
	//motor[DriveR2] = 127;
	//motor[DriveR3] = 127;
	TurnLeft();

	//motor[LiftLD] = 127;
	//motor[LiftRD] = 127;
	ArmUp();
	sleep(500);


	//motor[DriveL1] = -127;
	//motor[DriveL2] = -127;
	//motor[DriveL3] = -127;
	//motor[DriveR1] = -127;
	//motor[DriveR2] = -127;
	//motor[DriveR3] = -127;
	MoveBackwards();

	//motor[LiftLD] = 127;
	//motor[LiftRD] = 127;
	ArmUp();


	sleep(1000);
	//motor[DriveL1] = 0;
	//motor[DriveL2] = 0;
	//motor[DriveL3] = 0;
	//motor[DriveR1] = 0;
	//motor[DriveR2] = 0;
	//motor[DriveR3] = 0;
	ZeroDrive();

	//motor[LiftLD] = 0;
	//motor[LiftRD] = 0;
	ZeroArm();

	//motor[ClawL] = -127;
	//motor[ClawR] = -127;
	ClawClose();
	sleep(50);
	//motor[ClawL] = 0;
	//motor[ClawR] = 0;
	ZeroClaw();

}

void AutonomousCubeLeft()
{
  ClawOpen();
	sleep(600);
	ZeroClaw();
	sleep(50);
	ClawClose();
	sleep(154);
	ZeroClaw();

	ArmDown();
	sleep(200);
	ZeroArm();

	MoveForward(850);

	ClawOpen();
	sleep(600);

	TurnLeft();

	ArmUp();
	sleep(500);


	MoveBackwards();

	ArmUp();


	sleep(1000);

	ZeroDrive();

	ZeroArm();

	ClawClose();
	sleep(50);

	ZeroClaw();
}

void AutonomousGeneric()
{
	// TODO impl this
}



// for movement
// 500 is 30 inches (including drifting) 1 square

// for turning
// 100 is 90/4
// 250 is 90

typedef enum k_autonomous_debug
{
	AUTONOMOUS_TURN_RIGHT = 0,
	AUTONOMOUS_TURN_LEFT,
	AUTONOMOUS_FORWARD,
	AUTONOMOUS_BACKWARD,
	AUTONOMOUS_ARM_UP,
	AUTONOMOUS_ARM_DOWN,
	AUTONOMOUS_LAST,
} autonomous_debug_t;

string autonomous_debug_strings[] =
{
	"AUTONOMOUS_TURN_RIGHT",
	"AUTONOMOUS_TURN_LEFT",
	"AUTONOMOUS_FORWARD",
	"AUTONOMOUS_BACKWARD",
	"AUTONOMOUS_ARM_UP",
	"AUTONOMOUS_ARM_DOWN",
};

#define AUTONOMOUS_DEBUG

#ifdef AUTONOMOUS_DEBUG

// debug autonomous
void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = false;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  bLCDBacklight = true;
}

// debug autonomous
task autonomous()
{
	int buttonsPressed = 0;

	int time;

	autonomous_debug_t mode;

	while(true)
	{

		do
		{
			sleep(100);
			buttonsPressed = nLCDButtons;

			if(buttonsPressed == 5)
			{
				mode = (mode + 1) % AUTONOMOUS_LAST;
			}
			else if(buttonsPressed == 1)
			{
				time -= 10;
			}
			else if(buttonsPressed == 4)
			{
				time += 10;
			}

			clearLCDLine(0);
			clearLCDLine(1);
			displayLCDString(0, 0, autonomous_debug_strings[mode]);
			displayLCDNumber(1, 0, time);
		}
		while(buttonsPressed != 2);

		if(mode == AUTONOMOUS_TURN_RIGHT)
		{
			TurnRight(time);
		}
		else if(mode == AUTONOMOUS_TURN_LEFT)
		{
			TurnLeft(time);
		}
		else if(mode == AUTONOMOUS_FORWARD)
		{
			MoveForward(time);
		}
		else if(mode == AUTONOMOUS_BACKWARD)
		{
			MoveBackwards(time);
		}
		else if(mode == AUTONOMOUS_ARM_UP)
		{
			ArmUp(time);
		}
		else if(mode == AUTONOMOUS_ARM_DOWN)
		{
			ArmDown(time);
		}
	}
}

#else

// ============================================================
// COMPETITION AUTONOMOUS
// ============================================================

// TODO: Fix this - cube left mode and fix the menu system to use correct types

typedef enum k_autonomous_mode
{
	AUTONOMOUS_GENERIC,
	AUTONOMOUS_CUBE_LEFT,
	AUTONOMOUS_CUBE_RIGHT,
} autonomous_mode_t;

string autonomous_mode_strings[] =
{
	"generic",
	"cube right",
	"cube left",
};

// global for the autonomous mode selected
// the default autonomous mode is generic
// TODO: change this if we need to
autonomous_mode_t g_autonomous_mode = AUTONOMOUS_GENERIC;

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = false;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  bLCDBacklight = true;

  int buttonsPressed = 0;
 	int lastButtons = 0;

 	bool buttonsEqual = 0;

	int time;

	while(true)
	{
		sleep(100);
		buttonsPressed = nLCDButtons;

		if(buttonsPressed == 2)
		{
			g_autonomous_mode = AUTONOMOUS_GENERIC;

		}
		else if(buttonsPressed == 1)
		{
			g_autonomous_mode = AUTONOMOUS_CUBE_LEFT;
		}
		else if(buttonsPressed == 4)
		{
			g_autonomous_mode = AUTONOMOUS_CUBE_RIGHT;
		}

		// display the currently selected mode from the list
		clearLCDLine(0);
		displayLCDString(0, 0, autonomous_mode_strings[g_autonomous_mode]);

		clearLCDLine(1);
		displayLCDString(1, 0, "Press button again to select...");

		if(lastButtons == buttonsPressed)
		{
			// when the button is pressed twice in a row it is selected
			clearLCDLine(0);
			displayLCDString(0, 0, autonomous_mode_strings[g_autonomous_mode]);
			clearLCDLine(1);
			displayLCDString(0, 0, "selected");
			break;
		}

		// use the new buttons if a new buttons was pressed
		lastButtons = buttonsPressed ? buttonsPressed : lastButtons;
	}
}

task autonomous()
{
	if(g_autonomous_mode == AUTONOMOUS_GENERIC)
	{
		AutonomousGeneric();
	}
	else if(g_autonomous_mode == AUTONOMOUS_CUBE_LEFT)
	{
		AutonomousCubeLeft();
	}
	else if(g_autonomous_mode == AUTONOMOUS_CUBE_RIGHT)
	{
		AutonomousCubeRight();
	}

}

#endif


task usercontrol()
{
  // User control code here, inside the loop
  bool closeSwitch = false;
	int toggle = 1;
	int clawTick = 0;

	//int encoderAcc = 0;

  //Clear Encoders
  SensorValue[ArmEncoder] = 0;

  string mainBattery, backupBattery;

  while (true)
  {
  	if (vexRT[Btn7L])
  	{
  		// clear the lcd display
	  	clearLCDLine(0);
			clearLCDLine(1);

			// Display the Primary Robot battery voltage
			displayLCDString(0, 0, "Primary: ");
			sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V'); //Build the value to be displayed
			displayNextLCDString(mainBattery);

			// Display the Backup battery voltage
			displayLCDString(1, 0, "Backup: ");
			sprintf(backupBattery, "%1.2f%c", BackupBatteryLevel/1000.0, 'V');    //Build the value to be displayed
			displayNextLCDString(backupBattery);

			// Short delay for the LCD refresh rate
			// do we really want to be doing this? this will block this thread -
			// resulting in us being unable to use the robot for this time period
			wait1Msec(100);
		}

  	//sprintf(lcdDisplay, "%f", SensorValue[ArmEncoder]);
    //encoderAcc += SensorValue[ArmEncoder];
    displayLCDNumber(0, 0, SensorValue[ArmEncoder]);
    //SensorValue[ArmEncoder] = 0;

    int l = (vexRT[Ch3] + vexRT[Ch1]);
		int r = (vexRT[Ch3] - vexRT[Ch1]);

		float flForward = vexRT[Ch3];
		float flSide = vexRT[Ch1];

		int clawSpeed = 127;
		//Left
		motor[DriveL1] = l;
		motor[DriveL2] = l;
		motor[DriveL3] = l;
		//Right
		motor[DriveR1] = r;
		motor[DriveR2] = r;
		motor[DriveR3] = r;
		//Arm
		if (vexRT[Btn5U]) //Up
		{
			motor[LiftRD] = 127;
			motor[LiftLD] = 127;
		}
		else if (vexRT[Btn5D]) //Down
		{
			motor[LiftRD] = -127;
			motor[LiftLD] = -127;
		}
		else //Stop
		{
			motor[LiftRD] = 0;
			motor[LiftLD] = 0;
		}
		//Claw
		if (vexRT[Btn6U]) //Open
		{
			if(clawTick > 1000)
			{
				clawSpeed -= MIN(50, clawTick);
			}
			motor[ClawR] = clawSpeed;
			motor[ClawL] = clawSpeed;
			closeSwitch = false;
			clawTick += 1;

		}
		else if (vexRT[Btn6D]) //Close
		{
			if(clawTick > 1000)
			{
				clawSpeed -= MIN(50, clawTick);
			}
			motor[ClawR] = -clawSpeed;
			motor[ClawL] = -clawSpeed;
			//closeSwitch = true;
			clawTick += 1;

		}
		else //Stop
		{
			clawTick = 0;
			if (closeSwitch == false)
			{
				motor[ClawR] = 0;
				motor[ClawL] = 0;
			}

		if (vexRT(Btn7U))
		{
			motor[ClawR] = -127;
			motor[ClawL] = -127;
		}
			/*else
			{
				motor[ClawR] = -60;
				motor[ClawL] = -60;
			}
			*/
		}
  }
}
