#pragma once

#include "API.h"

// commonly used functions can be defined here

namespace Log
{
	inline void Print(const char *text)
	{
		//static Mutex mu = mutexCreate();

		//mutexTake(mu, -1);
		//delay(1000);
		printf(text);
		//delay(1000);
		//mutexGive(mu);
	}
};

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

template<class T, unsigned size = 10U>
class Stack
{
    T data[size];
    T *top_ptr;

public:
    Stack()
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
