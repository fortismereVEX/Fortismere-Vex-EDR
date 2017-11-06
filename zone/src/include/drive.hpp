#pragma once

#include "main.hpp"

#include "lcd.hpp"

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
	static encoder enc_left;
	static encoder enc_right;

	static float requested_left;
	static float requested_right;

	// TODO: these need to be left + right
	static float k_p;
	static float k_i;
	static float k_d;

	static float integral;
	static float last_error;


public:

	static void initialize()
	{
		enc_left.init(2, 3);
		enc_right.init(4, 5);
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

		int r = (fwd - lr);
		int l = (fwd + lr);

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

	static void pid_frame()
	{
		float pid_value = enc_left.get();
		float error = pid_value - requested_left;

		if(k_i != 0.0f)
		{
			if(abs(error) < 50.0f)
			{
				integral = integral + error;
			}
			else
			{
				integral = 0.0f;
			}
		}
		else
		{
			integral = 0.0f;
		}

		float derivative = error - last_error;
		last_error = error;

		float new_drive = (k_p * error) + (k_i * integral) + (k_p * derivative);

		clamp(new_drive, -127.0f, 127.0f);

		motorSet(2, new_drive);
		motorSet(3, new_drive);

	}

	static void run_frame()
	{
		run_drive();

		run_intake();

		run_lift();
	}
};
