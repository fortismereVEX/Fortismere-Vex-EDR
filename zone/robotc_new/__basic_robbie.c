#pragma config(Sensor, in1,    potent_left_lift, sensorPotentiometer)
#pragma config(Sensor, in2,    potent_right_lift, sensorPotentiometer)
#pragma config(Sensor, in3,    potent_arm,     sensorPotentiometer)
#pragma config(Sensor, in8,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  sonar_lift,     sensorSONAR_cm)
#pragma config(Motor,  port1,           roller,        tmotorNone, openLoop)
#pragma config(Motor,  port2,           left_middle,   tmotorNone, openLoop)
#pragma config(Motor,  port3,           arm_left,      tmotorNone, openLoop)
#pragma config(Motor,  port4,           left_front_back, tmotorNone, openLoop)
#pragma config(Motor,  port5,           lift_left,     tmotorNone, openLoop)
#pragma config(Motor,  port6,           arm_right,     tmotorNone, openLoop)
#pragma config(Motor,  port7,           right_front_back, tmotorNone, openLoop)
#pragma config(Motor,  port8,           lift_right,    tmotorNone, openLoop)
#pragma config(Motor,  port9,           right_middle,  tmotorNone, openLoop)
#pragma config(Motor,  port10,          mogo,          tmotorNone, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "robotc_base_include.c"

// utilitys for numbers
// unfortunately we do not have the safety of C++ templates...

#define max(x, a) (x > a ? x : a)
#define min(x, a) (x < a ? x : a)
#define clamp(x, a, b) (min(b, max(a, x)))

// These values are exposed here to allow helper functions to access them
float requested_left  = 0.0;
float requested_right = 0.0;

float current_left  = 0.0;
float current_right = 0.0;

float integral_left;
float integral_right;

float last_error_left;
float last_error_right;

float max_speed = 60.0;

void get_requested_delta(float *left, float *right) {
    int r = (vexRT[Ch1] - vexRT[Ch3]);
    int l = (vexRT[Ch1] + vexRT[Ch3]);

    *left  = abs(l) > 20 ? l : 0;
    *right = abs(r) > 20 ? r : 0;
}

float step_pid(float  constant_p,
               float  constant_i,
               float  constant_d,
               float  requested,
               float  current_value,
               float *last_error,
               float *integral) {
    float error = current_value - requested;

    float derivitive = error - *last_error;
    *last_error      = error;

    if (constant_i != 0 && abs(error) < 50) {
        *integral = *integral + error;
    } else {
        *integral = 0;
    }

    return constant_p * error +
           constant_i * *integral +
           constant_d * derivitive;
}

/*
task pid_loop() {
    // clear out the encoder
    //SensorValue[left_encoder]  = 0;
    //SensorValue[right_encoder] = 0;

    current_left  = 0;
    current_right = 0;

    requested_left  = 0;
    requested_right = 0;

    last_error_left  = 0;
    last_error_right = 0;

    integral_left  = 0;
    integral_right = 0;

    bool reset = true;

    while (true) {
        current_left  = SensorValue[left_encoder];
        current_right = SensorValue[right_encoder];

        // Since pid is only used in autonomous this code is commented out to prevent future problems.
        /*
        // get new usercontrol values here
        float delta_requested_left  = 0;
        float delta_requested_right = 0;

        get_requested_delta(&delta_requested_left, &delta_requested_right);


        if (delta_requested_left == 0 && delta_requested_right == 0) {
            if (reset) {
                requested_left             = 0;
                requested_right            = 0;
                SensorValue[left_encoder]  = 0;
                SensorValue[right_encoder] = 0;

                last_error_left  = 0;
                last_error_right = 0;

                reset = false;
            }
        } else {
            requested_left += delta_requested_left / 20;
            requested_right += delta_requested_right / 20;

            reset = true;
        }


        float new_left  = step_pid(1, 0.0, 1,
                                  requested_left,
                                  current_left,
                                  &last_error_left,
                                  &integral_left);
        float new_right = step_pid(1, 0.0, 1,
                                   requested_right,
                                   current_right,
                                   &last_error_right,
                                   &integral_right);

        if (abs(new_left) < 20) new_left = 0;
        if (abs(new_right) < 20) new_right = 0;

        writeDebugStreamLine("r: %.2f %.2f c: %.2f %.2f e: %.2f %.2f", requested_left, requested_right, current_left, current_right, last_error_left, last_error_right);

        new_left  = clamp(new_left, -max_speed, max_speed);
        new_right = clamp(new_right, -max_speed, max_speed);

        motor[right_middle]       = -new_right;
        motor[right_front_back] = -new_right;

        motor[left_middle]       = -new_left;
        motor[left_front_back] = -new_left;
	}
}
*/

float lift_requested_left = 3000;
float lift_requested_right = 3000;

bool lift_disabled = false;

task lift_pid_loop() {
	SensorValue[potent_left_lift] = 0;
	SensorValue[potent_right_lift] = 0;

	float current_left;
	float current_right;

	float last_error_left;
	float last_error_right;

	float integral_left = 0.0;
	float integral_right = 0.0;

	while(true) {
		if(lift_disabled) {
			motor[lift_left] = 0;
			motor[lift_right] = 0;
			continue;

		}

		current_left = SensorValue[potent_left_lift];
		//current_right = 4000 - SensorValue[potent_right_lift];
		current_right = current_left;

		float new_left = step_pid(0.2, 0.0, 0.1,
								  lift_requested_left,
								  current_left,
								  &last_error_left,
								  &integral_left);


		float new_right = step_pid(0.2, 0.0, 0.1,
								  lift_requested_right,
								  current_right,
								  &last_error_right,
								  &integral_right);

		writeDebugStream("r: %.2f c: %.2f n: %.2f\n", lift_requested_left, current_left, new_left);

		new_left = clamp(new_left, -127, 127);
		new_right = clamp(new_right, -127, 127);

		motor[lift_left] = -new_left;
		motor[lift_right] = new_right;

		wait1Msec(25);
	}
}

float arm_requested;

void arm_request_value_delta(float delta) {
	arm_requested += delta;

	arm_requested = clamp(arm_requested, 0, 0);
}

task arm_pid_loop() {
	float current;

	float last_error;

	float integral = 0.0;

	while(true) {
		current = SensorValue[potent_arm];

		float new_value = step_pid(0.2, 0.0, 0.1,
								  arm_requested,
								  current,
								  &last_error,
								  &integral);

		new_value = clamp(new_value, -127, 127);

		motor[arm_left] = -new_value;
		motor[arm_right] = new_value;

		wait1Msec(25);
	}
}



float lift_cone_delta = 200;

void lift_request_value_delta(float delta) {
	lift_requested_left -= delta;
	lift_requested_right -= delta;

	lift_requested_left = clamp(lift_requested_left, 2100, 3800);
	lift_requested_right = clamp(lift_requested_right, 2100, 3800);
}

void init() {
    stop_tasks_between_mode = true;
}

void forward(float amount) {
    requested_right -= amount;
    requested_left += amount;
}
void turn(float amount) {
    requested_right += amount;
    requested_left += amount;
}

bool at_dest(float threshold) {
    if (abs(requested_left - current_left) < threshold &&
        abs(requested_right - current_right) < threshold) {
        integral_left  = 0;
        integral_right = 0;
        return true;
    }

    return false;
}

void wait_for_dest(int timeout_ticks) {
    int curr = timeout_ticks;
    while (!at_dest(20) && curr >= 0) {
        sleep(25);
        --curr;
        //clearLCDLine(0);
        //displayLCDNumber(0, 0, curr, 2);
    }

    if (curr == 0) {
        displayLCDString(0, 0, "MISSED!!");
        requested_left   = current_left;
        requested_right  = current_right;
        last_error_left  = 0;
        last_error_right = 0;
    }
}

void lift(int power) {
	motor[lift_left] = power;
	motor[lift_right] = -power;
}

// positive for up, negative for down
void arm(int power) {
    motor[arm_left] = -power;
    motor[arm_right] = power;
}

void claw(int power) {
    motor[roller] = power;
}

task auton() {
    // Make sure the pid loop is restarted properly
    // This will get rid of residue
    //stopTask(pid_loop);
    //startTask(pid_loop);

    // Clear other sensors that we might use in the future
    //SensorValue[claw_potent]       = 0;
    //SensorValue[left_line_sensor]  = 0;
    //SensorValue[right_line_sensor] = 0;

    // potential for new
    int auton_option = 0;

    bLCDBacklight = true;
    clearLCDLine(0);

    switch (auton_option) {
    case 0:
		break;
    }

    // Stop pid and stop all motors to prevent drifting around
    //stopTask(pid_loop);
    all_motors_off();
}

enum {
	autostack_requested = 0,
	autostack_go_up,
	autostack_top_reached,
	autostack_release_cone,
	autostack_go_down_lift_up,
	autostack_go_down,
	autostack_finished,
};
int autostack_state = autostack_finished;

void autostack_frame() {
	if(autostack_state == autostack_finished) return; // nothing to do

	if(autostack_state == autostack_requested) autostack_state = autostack_go_up;

	if(autostack_state == autostack_go_up) {
		if(SensorValue[sonar_lift] < 20) {
			motor[lift_left] = 127;
			motor[lift_right] = -127;
		} else {
			motor[lift_left] = -50;
			motor[lift_right] = 50;

			autostack_state = autostack_top_reached;
		}
	}

	if(autostack_state == autostack_top_reached) {
		if(SensorValue[potent_arm] > 2500) {
			motor[arm_left] = 60;
			motor[arm_right] = -60;
		} else {
			motor[arm_left] = -20;
			motor[arm_right] = 20;

			autostack_state = autostack_release_cone;
		}
	}

	if(autostack_state == autostack_release_cone) {
		if(SensorValue[sonar_lift] > 10) {
			motor[roller] = -127;
		} else {
			autostack_state = autostack_go_down_lift_up;
		}
	}

	if(autostack_state == autostack_go_down_lift_up) {
		if(SensorValue[sonar_lift] < 20) {
			motor[lift_left] = 127;
			motor[lift_right] = -127;
		} else {
			motor[lift_left] = 0;
			motor[lift_right] = 0;

			autostack_state = autostack_go_down;
		}
	}

	if(autostack_state == autostack_go_down) {
		if(SensorValue[sonar_lift] > 12) {
			motor[lift_left] = -127;
			motor[lift_right] = 127;
		} else {
			motor[lift_left] = 0;
			motor[lift_right] = 0;

			autostack_state = autostack_finished;
		}
	}

	if(autostack_state == autostack_finished) {
		motor[lift_left] = 0;
		motor[lift_right] = 0;

		motor[arm_left] = 0;
		motor[arm_right] = 0;
	}

}


// Some state variables for persistance
enum {
    roller_open   = 0,
    roller_closed = 1,
};

int requested_roller = roller_closed;
float claw_change = 0.0;

enum {
	mogo_down = 0,
	mogo_up = 1,
};
int requested_mogo = mogo_up;


task user_control() {
    sleep(500);

    startTask(lift_pid_loop);

    while (true) {
        int r = (vexRT[Ch3] - vexRT[Ch1]);
        int l = (vexRT[Ch3] + vexRT[Ch1]);

        // account for deadzone
        if (abs(l) < 20) l = 0;
        if (abs(r) < 20) r = 0;

        motor[right_middle]     = -r;
        motor[right_front_back] = -r;

        motor[left_middle]     = l;
        motor[left_front_back] = l;

        autostack_frame();

        if(vexRT[Btn8L] == true) {
        	autostack_state = autostack_finished;
        }

        // check whether autostack is active in order to see whether the user can do controls right now
        if(autostack_state != autostack_finished) continue;

        if(vexRT[Btn7R] == true) {
        	lift_disabled = !lift_disabled;
        }

        // move the arm
        int lift_sign = 0;
        if (vexRT[Btn6U] == true) {
            lift_sign = -1;
        } else if (vexRT[Btn6D] == true) {
            lift_sign = 1;
        }

        //motor[lift_left] = 127 * -lift_sign;
        //motor[lift_right] = 127 * lift_sign;

		lift_request_value_delta(lift_sign * 2);

        int arm_sign = 0;
        if (vexRT[Btn5D] == true) {
            arm_sign = 1;
        } else if (vexRT[Btn5U] == true) {
            arm_sign = -1;
        }

        motor[arm_left] = 127 * -arm_sign;
        motor[arm_right] = 127 * arm_sign;

        //float current_potent = SensorValue[claw_potent]
        //float change = current_potent - claw_change;
        //claw_change = current_potent;

        bool roller_stopped = false;

        //if(change == 0) roller_stopped = true;

        // move the roller
        requested_roller = roller_closed;

        if (vexRT[Btn8U] == true) {
            requested_roller = roller_open;
        }

        if (requested_roller == roller_open) {
			motor[roller] = -127;
        } else if (requested_roller == roller_closed) {
            motor[roller] = 30;
        } else {
    		motor[roller] = 0;
        }

        if(vexRT[Btn7U] == true) {
        	requested_mogo = mogo_up;
			motor[mogo] = 127;
        } else if(vexRT[Btn7D] == true) {
      		requested_mogo = mogo_down;
			motor[mogo] = -127;
        } else if(requested_mogo == mogo_down) {
     	  	motor[mogo] = -15; // constant power holds the stack upright
    	} else {
    		motor[mogo] = 0;
    	}

    	if(vexRT[Btn8R] == true) {
    		autostack_state = autostack_requested; // LETS GO BOIS
    	}

    	clearLCDLine(0);
    	clearLCDLine(1);

    	bLCDBacklight = true;
    	displayLCDNumber(0, 0, SensorValue[potent_left_lift]);
    	displayLCDNumber(1, 0, 4000 - SensorValue[potent_right_lift]);

    	displayLCDNumber(0, 7, SensorValue[sonar_lift]);
    	displayLCDNumber(1, 7, SensorValue[potent_arm]);
    }
}
