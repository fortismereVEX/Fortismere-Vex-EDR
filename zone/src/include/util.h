#pragma once

#include "API.h"

// commonly used functions can be defined here

template<typename T>
inline T clamp(T x, T min, T max)
{
	return x > max ? max : x < min ? min : x;
}
template<typename T>
inline T abs(T x)
{
	return x < 0 ? -x : x;
}

template<typename T>
inline T Max(T x, T y)
{
	return x > y ? x : y;
}

template<typename T>
inline T Min(T x, T y)
{
	return x < y ? x : y;
}

#define STACK_SIZE_CHECK

template<typename T, unsigned size = 32>
class Stack
{
	T data[size];

	T *pTop;

public:
	Stack()
	{
		pTop = data;
	}

	// temporary DO NOT HOLD AROUND
	T &pop()
	{
		T &ret = *(pTop--);
		return ret;
	}

	void push(T val)
	{
		#ifdef STACK_SIZE_CHECK
		if((unsigned long long)pTop - (unsigned long long)data >= size)
		{
			printf("STACK DATA OVERFLOW\n");
		}
		#endif
		*(++pTop) = val;
	}

	T &top()
	{
		return *pTop;
	}
};
