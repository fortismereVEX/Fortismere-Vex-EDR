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

	//LCD::SetLcdUpdateInterval(5);

	//LCD::DisplayEnumOptions<Options>(Options::Pid, OptionsStrings, &OptionsCallback);

	while (true) {
		// maximum change for pidRequestedValue will be 127/4*20, around 640 counts
		// per second
		// free spinning motor is 100rmp so 1.67 rotations per second
		// 1.67 * 360 counts is 600

		//EncoderRight->requestedValue = (joystickGetAnalog(1, 2) / 4);
		//EncoderLeft->requestedValue = (joystickGetAnalog(1, 2) / 4);

		int l = (joystickGetAnalog(1, 1) - joystickGetAnalog(1, 3));
		int r = (joystickGetAnalog(1, 1) + joystickGetAnalog(1, 3));

		// drive
		motorSet(DriveRight1, r);
		motorSet(DriveRight2, r);

		motorSet(DriveLeft1, l);
		motorSet(DriveLeft2, l);

		if(joystickGetDigital(1, 5, JOY_DOWN))
		{
			// up
			motorSet(LiftRight1, -127);
			motorSet(LiftRight2, -127);

			motorSet(LiftLeft1, 127);
			motorSet(LiftLeft2, 127);
		}
		else if(joystickGetDigital(1, 6, JOY_DOWN))
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

		// Claw
		if(joystickGetDigital(1, 6, JOY_UP))
		{
			// open
			motorSet(ClawLeft, 60);
			motorSet(ClawRight, -60);
		}
		else if(joystickGetDigital(1, 5, JOY_UP))
		{
			// close
			motorSet(ClawLeft, -60);
			motorSet(ClawRight, 60);
		}
		else
		{
			motorSet(ClawLeft, 0);
			motorSet(ClawRight, 0);
		}

		delay(1);
	}
}
