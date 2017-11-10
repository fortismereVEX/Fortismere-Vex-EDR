#include "drive.hpp"

bool drive::in_demo;
bool drive::in_recording = false;

recorder *drive::rec = nullptr;
replay *drive::rep = nullptr;

//encoder drive::enc_left;
//encoder drive::enc_right;

ime drive::ime_left;
ime drive::ime_right;

char drive::power_left;
char drive::power_right;

float drive::k_p = 0.6f;
float drive::k_i = 0.001f;
float drive::k_d = 0.55f;

float drive::integral_left = 0.0f;
float drive::integral_right	 = 0.0f;
float drive::last_error_right = 0.0f;
float drive::last_error_left = 0.0f;
