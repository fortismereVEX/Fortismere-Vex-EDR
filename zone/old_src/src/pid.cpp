#include "../include/pid.h"

static int pidRunning = 1;

void pidTask(void *param)
{
	// These could be constants but leaving
	// as variables allows them to be modified in the debugger "live"
	auto *setup = (EncoderSetup *)param;

	float pidSensorCurrentValue;

    float pidError = 1;
    float pidLastError = 1;
    float pidIntegral = 1;
    float pidDerivative = 1;
    float pidDrive = 1;

    // If we are using an encoder then clear it
    encoderReset(setup->encoder);

    // Init the variables - thanks Glenn :)
    pidLastError = 0;
    pidIntegral = 0;

	//printf("0x%X kp %f ki %f kd %f\n", setup->encoder, setup->constant_p, setup->constant_i, setup->constant_d);

    while (true) {
		float pid_Kp = setup->constant_p;
		float pid_Ki = setup->constant_i;
		float pid_Kd = setup->constant_d;

		// Is PID control active ?z
		if (pidRunning) {
			// Read the sensor value and scale
			pidSensorCurrentValue = encoderGet(setup->encoder) * PID_SENSOR_SCALE;

			printf("Encoder 0x%X v: %+.6f r: %+.6f p: %+.6f i: %+.6f d: %+.6f\n", setup->encoder, pidSensorCurrentValue, setup->requestedValue, pidError, pidIntegral, pidDerivative);

			// calculate error
			pidError = pidSensorCurrentValue - setup->requestedValue;

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
			pidDrive = (pid_Kp * pidError) + (pid_Ki * pidIntegral) + (pid_Kd * pidDerivative);

			// limit drive
			if (pidDrive > PID_DRIVE_MAX)
				pidDrive = PID_DRIVE_MAX;
			if (pidDrive < PID_DRIVE_MIN)
				pidDrive = PID_DRIVE_MIN;

			// send to motor
			// TODO: this needs to be intergrated with other stuff
			int count = setup->motorCount;
			for(int i = 0; i < count; i++)
			{
				motorSet(setup->motors[i], pidDrive);
			}
			//motor[PID_MOTOR_INDEX] = pidDrive * PID_MOTOR_SCALE;
		} else {
			//printf("CLEAR !!111!!\n\n\n\n\n");
			// clear all
			pidError = 0;
			pidLastError = 0;
			pidIntegral = 0;
			pidDerivative = 0;
			int count = setup->motorCount;
			setup->requestedValue = 0;
			for(int i = 0; i < count; i++)
			{
				motorSet(setup->motors[i], 0);
			}
		}

      // Run at 50Hz
      delay(25);
    }
}
