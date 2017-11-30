#include "drive.hpp"

bool drive::in_demo;
bool drive::in_recording = false;

int drive::last_time = 0;

#if defined(ROBOT_ROBBIE)
pid_helper_real<ime> drive::pid_drive_left(ime(0), 0.9f, 0.2f, 5.0f, 0.0f);
pid_helper_real<ime> drive::pid_drive_right(ime(1), 0.9f, 0.2f, 5.0f, 10.0f);
#elif defined(ROBOT_SAM)
pid_helper_real<ime> drive::pid_drive_left(ime(0), 0.9f, 0.2f, 5.0f, 0.0f);
pid_helper_real<ime> drive::pid_drive_right(ime(1), 0.9f, 0.2f, 5.0f, 10.0f);
#endif

bool drive::slow_mode = false;

char drive::power_left;
char drive::power_right;
