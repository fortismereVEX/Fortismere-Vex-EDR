#pragma once

#include "main.hpp"

#include "lcd.hpp"

#define DRIVE_DEBUG

// TODO: recorder + replay should be the same class

class recorder
{
	char *buffer;
	unsigned size;
	unsigned pos;

	void resize(unsigned additional)
	{
		size += additional;
		buffer = (char *)realloc(buffer, size);
	}

public:
	recorder() : buffer(nullptr), size(256), pos(-1)
	{
		buffer = (char *)malloc(size);
	}

	void write(char type, char value)
	{
		// write 1 byte for the type followed by a byte for the value
		// this may not be required and we could think about writing nibbles
		// for the type or for both
		if(pos + 2 >= size)
		{
			resize(64);
		}

		buffer[++pos] = type;
		buffer[++pos] = value;
	}
	bool save_to_disk(const char *filename)
	{
		// write a trailing 00 00 pair
		write(0, 0);

		FILE *f = fopen(filename, "w");

		if(f == nullptr)
		{
			lcd::displayMessage("w: unable to write to file");
			return false;
		}

		int wrote = fwrite(buffer, 1, size, f);

		if(wrote < size)
		{
			lcd::displayMessage("w: unable to write all bytes of file");
		}

		fclose(f);
	}

};

class replay
{
	char *buffer;
	int size;
	int position;

public:
	replay(const char *filename) : position(-1), size(0), buffer(nullptr)
	{
		FILE *f = fopen(filename, "r");

		if(f == nullptr)
		{
			lcd::displayMessage("r: unable to open file");
		}

		fseek(f, 0, SEEK_END);
		size = ftell(f);

		// rewind
		fseek(f, 0, SEEK_SET);

		// now allocate enough space for every other byte
		buffer = (char *)malloc((size + 1) / 2);

		for(int i = 0; i < size; i++)
		{
			buffer[i] = fread(buffer, 1, size, f);
		}
	}

	void read(char *type, char *byte)
	{
		type = &buffer[++position];
		byte = &buffer[++position];
	}
};

class drive
{
	// are we replaying joystick / button commmands
	static bool in_demo;
	static bool in_recording;

	static recorder *rec;
	static replay *rep;

	// pid values
	//static encoder enc_left;
	//static encoder enc_right;

	static int last_time;

	static bool pid_enabled;

	static ime ime_left;
	static ime ime_right;

	static char power_left;
	static char power_right;

	static float requested_left;
	static float requested_right;

	// TODO: these need to be left + right
	static float k_p;
	static float k_i;
	static float k_d;

	static float integral_left;
	static float integral_right;
	static float last_error_left;
	static float last_error_right;

	static float get_rpm_value(char power)
	{
		return ((float)power / 127.0f) * 800.0f;
	}

	static char get_power_value(float rpm)
	{
		return (char)((rpm / 800.0f) * 127.0f);
	}

	static int get_delta_time()
	{
		int current_time = millis();
		int dt = current_time - last_time;
		last_time = current_time;
		return dt;
	}

public:
	static void initialize()
	{
		auto inited = imeInitializeAll();
		lcd::printf("%d imes", inited);

		ime_left.init(0);
		ime_right.init(1);
	}

	static int get_joystick_analog(int axis)
	{
		if(in_demo == true)
		{
			// return next byte
			return 0;
		}

		char value = joystickGetAnalog(1, axis);

		// account for dead space
		if(abs(value) < 20) value = 0;

		if(in_recording == true)
		{
			// write to buffer
		}

		return value;
	}
	static bool get_joystick_digital(int group, int direction)
	{
		if(in_demo == true)
		{
			// return next byte
			return 0;
		}
		else if(in_recording == true)
		{
			char value = joystickGetDigital(1, group, direction);
			// store value in buffer
			return value;
		}
		return joystickGetDigital(1, group, direction);
	}

	static void run_drive()
	{
		int fwd = get_joystick_analog(1);
		int lr = get_joystick_analog(3);

		power_right = (fwd - lr);
		power_left = (fwd + lr);
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

	static void pid_frame()
	{
		if(get_joystick_digital(8, JOY_UP))
		{
			pid_enabled = true;
		}
		else if(get_joystick_digital(8, JOY_DOWN))
		{
			pid_enabled = false;
		}
		if(pid_enabled)
		{
			float left_rpm = get_rpm_value(power_left);
			float right_rpm = get_rpm_value(power_right);

			float left_real_rpm = ime_left.get_value(nullptr);
			float right_real_rpm = ime_right.get_value(nullptr);

			#if defined(DRIVE_DEBUG)
			lcdPrint(uart1, 2, "w: %.1f a: %.1f", right_rpm, right_real_rpm);
			//printf("w: %.1f a: %.1f\n", right_rpm, right_real_rpm);
			#endif

			float left_error = left_rpm - left_real_rpm;
			float right_error = right_rpm - right_real_rpm;

			#if defined(DRIVE_DEBUG)
			printf("l: %.1f r: %.1f\n", left_error, right_error);
			#endif

			if(k_i != 0.0f)
			{
				int dt = get_delta_time();
				if(abs(left_error) < 30.0f)
				{
					integral_left = integral_left + left_error * dt;
				}
				else
				{
					integral_left = 0.0f;
				}

				if(abs(right_error) < 30.0f)
				{
					integral_right = integral_right + right_error * dt;
				}
				else
				{
					integral_right = 0.0f;
				}
			}
			else
			{
				integral_right = 0.0f;
				integral_left = 0.0f;
			}

			float derivative_right = right_error - last_error_right;
			float derivative_left = left_error - last_error_left;
			last_error_right = right_error;
			last_error_left = left_error;

			float new_drive_left = (k_p * left_error) + (k_i * integral_left) + (k_p * derivative_left);
			float new_drive_right = (k_p * right_error) + (k_i * integral_right) + (k_p * derivative_right);

			power_left = get_power_value(new_drive_left);
			power_right = get_power_value(new_drive_right);
		}
	}

	static void finalise()
	{
		#if defined(DRIVE_DEBUG)
		lcdPrint(uart1, 1, "l: %d r: %d", power_left, power_right);
		//printf("w: %.1f a: %.1f\n", right_rpm, right_real_rpm);
		#endif

		// front left
		motorSet(2, power_left);
		// back left
		motorSet(3, power_left);

		motorSet(8, power_left);

		// front right
		motorSet(4, power_right);
		// back right
		motorSet(5, power_right);

		motorSet(9, power_right);
	}

	static void run_frame()
	{
		run_drive();

		run_intake();

		run_lift();

		pid_frame();

		finalise();
	}
};
