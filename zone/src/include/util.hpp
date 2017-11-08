#pragma once

#include "main.hpp"

template<typename T>
inline T min(T a, T b)
{
	if(a < b)
		return a;
	return b;
}

template<typename T>
inline T max(T a, T b)
{
	if(a > b)
		return a;
	return b;
}

template<typename T>
inline T abs(T val)
{
	if(val < 0)
		return -val;

	return val;
}

template<typename T>
inline T clamp(T x, T min, T max)
{
	if(x < min)
		return min;
	if(x > max)
		return max;

	return x;
}

template<class T, unsigned size = 10U>
class stack
{
    T data[size];
    T *top_ptr;

public:
    stack()
    {
        top_ptr = data;
    }

    T &top()
    {
        return *top_ptr;
    }

    T &pop()
    {
        return *(top_ptr--);
    }

    void push(T elem)
    {
        *(++top_ptr) = elem;
        return;
    }
};

class mutex
{
	Mutex m;

public:
	mutex()
	{
		m = mutexCreate();
	}

	void lock()
	{
		mutexTake(m, -1);
	}

	void unlock()
	{
		mutexGive(m);
	}
};


class encoder
{
	Encoder e;
public:
	encoder() = default;

	void init(int top, int bot, bool reverse = false)
	{
		e = encoderInit(top, bot, reverse);
		encoderReset(e);
	}

	~encoder()
	{
		encoderShutdown(e);
	}

	void reset()
	{
		encoderReset(e);
	}

	int get()
	{
		encoderGet(e);
	}
};

class ime
{
	char ime_address;
public:

	ime() = default;

	void init(char address)
	{
		ime_address = address;
		imeReset(ime_address);
	}

	float get_value(bool *success)
	{
		int value = 0;
		bool result = imeGetVelocity(ime_address, &value);

		if(success != nullptr)
		{
			*success = result;
		}

		return value * 24.5;
	}


};
