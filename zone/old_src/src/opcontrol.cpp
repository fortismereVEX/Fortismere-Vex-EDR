#include "main.h"
#include "pid.h"
#include "lcdmenu.h"

#include "stdlib.h"
#include "string.h"

enum class Options
{
	Voltage,
	Pid,
	Last,
};
const char *OptionsStrings[] =
{
	"Voltage",
	"Pid",
};

void OptionsCallback(Options chosen)
{
	if(chosen == Options::Voltage)
	{
		char buffer[255];
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, 16, "M: %.2f B: %.2f", (float)powerLevelMain() / 1000.0f, (float)powerLevelBackup() / 1000.0f);
		LCD::DisplayMessage(2000, buffer);
	}
}

void operatorControl() {

	while(g_Ready == false)
	{
		delay(100);
	}

	//LCD::SetLcdUpdateInterval(5);

	printf("opcontrol begins\n");

	LCD::DisplayEnumOptions<Options>(Options::Pid, OptionsStrings, &OptionsCallback);


	analogCalibrate(Sensors::ClawPotent);

	enum CState
	{
		Opening,
		Open,
		Closing,
		Closed,
	};

	int clawState = CState::Opening;
	int lastClawValue = 0;
	int currentClawValue = 0;

	while (true) {
		// maximum change for pidRequestedValue will be 127/4*20, around 640 counts
		// per second
		// free spinning motor is 100rmp so 1.67 rotations per second
		// 1.67 * 360 counts is 600

		//EncoderRight->requestedValue = (joystickGetAnalog(1, 2) / 4);
		//EncoderLeft->requestedValue = (joystickGetAnalog(1, 2) / 4);

		int l = (joystickGetAnalog(1, 1) - joystickGetAnalog(1, 3));
		int r = (joystickGetAnalog(1, 1) + joystickGetAnalog(1, 3));

		//printf("%d\n", analogReadCalibrated(Sensors::ClawPotent));

		// drive
		motorSet(DriveRight1, r);
		motorSet(DriveRight2, r);

		motorSet(DriveLeft1, l);
		motorSet(DriveLeft2, l);

		if(joystickGetDigital(1, 5, JOY_UP))
		{
			// up
			motorSet(LiftRight1, -127);
			motorSet(LiftRight2, -127);

			motorSet(LiftLeft1, 127);
			motorSet(LiftLeft2, 127);
		}
		else if(joystickGetDigital(1, 5, JOY_DOWN))
		{
			// down
			motorSet(LiftRight1, 127);
			motorSet(LiftRight2, 127);

			motorSet(LiftLeft1, -127);
			motorSet(LiftLeft2, -127);
		}
		else
		{
			// stop
			motorSet(LiftRight1, 0);
			motorSet(LiftRight2, 0);

			motorSet(LiftLeft1, 0);
			motorSet(LiftLeft2, 0);
		}

		lastClawValue = currentClawValue;
		currentClawValue = analogReadCalibrated(Sensors::ClawPotent);

		// close is values > 0
		// open is when the value is 0

		// Claw
		if(joystickGetDigital(1, 6, JOY_UP))
		{
			// open
			clawState = CState::Opening;
			motorSet(ClawLeft, 60);
			motorSet(ClawRight, -60);
		}
		else if(joystickGetDigital(1, 6, JOY_DOWN))
		{
			// close
			clawState = CState::Closing;

			motorSet(ClawLeft, -60);
			motorSet(ClawRight, 60);
		}
		else
		{
			motorSet(ClawLeft, 0);
			motorSet(ClawRight, 0);
		}

		/*

		if(clawState == CState::Opening)
		{
			// we are trying to open
			if(currentClawValue < 50)
			{
				motorSet(ClawLeft, 30);
				motorSet(ClawRight, -30);
			}
			else if(currentClawValue > 20 || currentClawValue > -20)
			{
				clawState = CState::Open;
			}
		}
		else if(clawState == CState::Closing)
		{
			// we are trying to close
			// slow thing down a bit
			if(abs(lastClawValue - currentClawValue) < 10)
			{
				// slow down
				motorSet(ClawLeft, -30);
				motorSet(ClawRight, 30);
			}
		}
		else if(clawState == CState::Closed || clawState == CState::Open)
		{
			motorSet(ClawLeft, 0);
			motorSet(ClawRight, 0);
		}

		*/

		delay(1);
	}
}
