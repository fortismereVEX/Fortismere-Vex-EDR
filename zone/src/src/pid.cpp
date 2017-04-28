#include "../include/pid.h"

// These could be constants but leaving
// as variables allows them to be modified in the debugger "live"
float pid_Kp = 2.0;
float pid_Ki = 0.04;
float pid_Kd = 0.1;

static int pidRunning = 1;
static float pidRequestedValue;

void pidTask(void *param)
{
	float pidSensorCurrentValue;

    float pidError;
    float pidLastError;
    float pidIntegral;
    float pidDerivative;
    float pidDrive;

    // If we are using an encoder then clear it
    encoderReset(pidEncoder);

    // Init the variables - thanks Glenn :)
    pidLastError = 0;
    pidIntegral = 0;

    while (true) {
      // Is PID control active ?
      if (pidRunning) {
        // Read the sensor value and scale
        pidSensorCurrentValue = encoderGet(pidEncoder) * PID_SENSOR_SCALE;

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
		// TODO: this needs to be intergrated with other stuff
		motorSet(PID_MOTOR_INDEX, pidDrive * PID_MOTOR_SCALE);
        //motor[PID_MOTOR_INDEX] = pidDrive * PID_MOTOR_SCALE;
      } else {
        // clear all
        pidError = 0;
        pidLastError = 0;
        pidIntegral = 0;
        pidDerivative = 0;
        motorSet(PID_MOTOR_INDEX, 0);
      }

      // Run at 50Hz
      delay(25);
    }
}
