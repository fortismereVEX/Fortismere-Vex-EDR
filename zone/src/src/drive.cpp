#include "drive.hpp"

bool drive::in_demo;
bool drive::in_recording = false;

//recorder *drive::rec = nullptr;
//replay *drive::rep = nullptr;

int drive::last_time = 0;

pid_helper<ime> drive::pid_drive_left(ime(0), 0.9f, 0.0f, 0.9f, 700.0f);
pid_helper<ime> drive::pid_drive_right(ime(1), 0.9f, 0.0f, 0.9f, 700.0f);

bool drive::pid_enabled = true;

char drive::power_left;
char drive::power_right;
