#pragma once

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
