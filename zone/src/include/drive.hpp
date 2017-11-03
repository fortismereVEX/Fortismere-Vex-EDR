#include "main.hpp"


class drive
{
	// are we replaying joystick / button commmands
	static bool in_demo;

public:

	static void initialize()
	{
	}

	static int get_joystick_analog(int axis)
	{
		if(in_demo == false)
			joystickGetAnalog(1, axis);
	}
	static bool get_joystick_digital(int group, int button)
	{
		if(in_demo == false)
			joystickGetDigital(1, group, button);
	}

	static void run_drive()
	{
		int l = (get_joystick_analog(1) - get_joystick_analog(3));
		int r = (get_joystick_analog(1) + get_joystick_analog(3));

		// front left
		motorSet(2, l);
		// back left
		motorSet(3, l);
		// front right
		motorSet(4, r);
		// back right
		motorSet(5, r);
	}

	static void run_intake()
	{
		if(get_joystick_digital(7, JOY_UP))
		{
			motorSet(6, 127);
			motorSet(7, -127);
		}
		else if(get_joystick_digital(7, JOY_DOWN))
		{
			motorSet(6, -127);
			motorSet(7, 127);
		}
		else
		{
			motorSet(6, 0);
			motorSet(7, 0);
		}
	}

	static void run_lift()
	{

	}

	static void run_frame()
	{
		run_drive();

		run_intake();

		run_lift();
	}
};
