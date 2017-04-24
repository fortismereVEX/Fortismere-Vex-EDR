pidIntegral = 0;

  while (true) {
    // Is PID control active ?
    if (pidRunning) {
      // Read the sensor value and scale
      pidSensorCurrentValue = SensorValue[PID_SENSOR_INDEX] * PID_SENSOR_SCALE;

      // calculate error
      pidError = pidSensorCurrentValue - pidRequestedValue;

      // integral - if Ki is not 0
      if (pid_Ki != 0) {
        // If we are inside controlable window then integrate the error
        if (abs(pidError) < PID_INTEGRAL_LIMIT)
          pidIntegral = pidIntegral + pidError;
        else
          pidIntegral = 0;
      } else
        pidIntegral = 0;

      // calculate the derivative
      pidDerivative = pidError - pidLastError;
      pidLastError = pidError;

      // calculate drive
      pidDrive = (pid_Kp * pidError) + (pid_Ki * pidIntegral) +
                 (pid_Kd * pidDerivative);

      // limit drive
      if (pidDrive > PID_DRIVE_MAX)
        pidDrive = PID_DRIVE_MAX;
      if (pidDrive < PID_DRIVE_MIN)
        pidDrive = PID_DRIVE_MIN;

      // send to motor
      motor[PID_MOTOR_INDEX] = pidDrive * PID_MOTOR_SCALE;
    } else {
      // clear all
      pidError = 0;
      pidLastError = 0;
      pidIntegral = 0;
      pidDerivative = 0;
      motor[PID_MOTOR_INDEX] = 0;
    }

    // Run at 50Hz
    wait1Msec(25);
  }
}

typedef enum k_pid_options
{
	PID_PROPORTIONAL = 0,
	PID_INTEGRAL,
	PID_DERIVATIVE,
	PID_LAST,
} pid_options_t;

string pid_options_strings[] =
{
	"PROPORTIONAL",
	"INTEGRAL",
	"DERIVATIVE",
};

task lcdMenu() {

	long mode;
	float *value;
	long buttonsPressed;
	string text;
	while(true) {
		wait1Msec(500);
		buttonsPressed = nLCDButtons;

		if(buttonsPressed == 5)
		{
			mode = (mode + 1) % PID_LAST;
			if(mode == PID_PROPORTIONAL) {
				value = &pid_Kp;
			}
			else if(mode == PID_INTEGRAL) {
				value = &pid_Ki;
			}
			else if(mode == PID_DERIVATIVE) {
				value = &pid_Kd;
			}
		}
		else if(buttonsPressed == 1)
		{
			*value += 0.01;
		}
		else if(buttonsPressed == 4)
		{
			*value -= 0.01;
		}

		clearLCDLine(0);
		clearLCDLine(1);
		displayLCDString(0, 0, pid_options_strings[mode]);
		sprintf(text, "%.5f", *value);
		displayLCDString(1, 0, text);
	}
}

task main() {
  // send the motor off somewhere
  pidRequestedValue = 1000;

  // start the PID task
  startTask(pidController);

  // start the menu task
  startTask(lcdMenu);

  // use joystick to modify the requested position
  while (true) {
    // maximum change for pidRequestedValue will be 127/4*20, around 640 counts
    // per second
    // free spinning motor is 100rmp so 1.67 rotations per second
    // 1.67 * 360 counts is 600

    pidRequestedValue = pidRequestedValue + (vexRT[Ch2] / 4);

    wait1Msec(50);
  }
}