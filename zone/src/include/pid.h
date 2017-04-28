#pragma once

#include "main.h"
#include "util.h"


#define PID_SENSOR_SCALE 1

#define PID_MOTOR_INDEX (MOTOR1)
#define PID_MOTOR_SCALE -1

#define PID_DRIVE_MAX 127
#define PID_DRIVE_MIN (-127)

#define PID_INTEGRAL_LIMIT 50

void pidTask(void *param);
