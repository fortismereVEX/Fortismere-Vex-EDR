#pragma once
#include <API.hpp>

// robot switch here
#define ROBOT_ROBBIE
//#define ROBOT_SAM

extern "C" {
void autonomous();
void initializeIO();
void initialize();

void operatorControl();
}
