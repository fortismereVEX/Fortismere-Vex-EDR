#pragma once

#include "main.h"
#include "util.h"

struct EncoderSetup
{
	float constant_p;
	float constant_i;
	float constant_d;

	Encoder encoder;

	float requestedValue;

	int motorCount;
	int motors[0];
};

// Allocates the correct amount of space for this encoderSetup
inline EncoderSetup *AllocateEncoderSetup(int motorCount)
{
	auto *allocated =  (EncoderSetup *)malloc(sizeof(EncoderSetup) + motorCount * sizeof(int));
	allocated->motorCount = motorCount;

	return allocated;
}

#define PID_SENSOR_SCALE 1

#define PID_MOTOR_SCALE -1

#define PID_DRIVE_MAX 127
#define PID_DRIVE_MIN (-127)

#define PID_INTEGRAL_LIMIT 50

void pidTask(void *param);
