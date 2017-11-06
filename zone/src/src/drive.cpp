#include "drive.hpp"

bool drive::in_demo;
bool drive::in_recording = false;

recorder *drive::rec = nullptr;
replay *drive::rep = nullptr;

encoder drive::enc_left;
encoder drive::enc_right;

float drive::k_p = 20.0f ;
float drive::k_i = 0.0f;
float drive::k_d = 2.0f;

float drive::integral = 0.0f;
float drive::last_error = 0.0f;
