#pragma once

#include "main.h"

#include "util.h"

#define MAX_DELAY (0xFFFFFFFF)

// this file defines a common interface to display options on the lcd display

// THE CODE IS CURRENTLY CONFIGURED TO USE UART1 AS THE DISPLAY PORT
// IF THIS IS CHANGED THEN CHANGE IT IN THE CODE!
// YOU *MUST* ALSO CHANGE THE CODE IN INIT.CPP

#define LCD_PORT uart2

namespace LCD
{
	enum LCDMode
	{
		Wait,
		Enum,
		RtUpdate,
		Message,
	};

	// define some state for the lcd

	// state access mutex - important for syncronisation
	extern Mutex g_mutex;

	extern Stack<LCDMode> *g_modeStack;

	// realtime update value
	extern float *g_rtvalue;
	extern float g_rtinterval;
	extern bool g_rthasminmax;
	extern float g_rtmax;
	extern float g_rtmin;

	extern int g_messageWait;

	// "translations" for enum types
	extern const char **g_strings;

	// message string data
	extern const char *g_message;

	// max value for enums
	extern int g_max;

	// callback for value changes
	extern void *g_callback;

	extern bool g_stateChanged;

	// this is the global task for the lcd menu
	void LcdTask(void *param);

	// Displays an option for an enumerical type
	// Enum must be an integral, starting at 0
	// max should be the total size of the enum (size not index!)
	// this can be easily done by adding an enum_last value to the end of your enum
	// callback gets called with the selected value
	// callback should not take a long time to execute - as it blocks the lcd thread!
	template<typename Enum>
	void DisplayEnumOptions(Enum max, const char **strings, void(*callback)(Enum val))
	{
		Log::Print("DisplayEnumOptions\n");

		if(LCD::g_mutex == nullptr)
		{
			LCD::g_mutex = mutexCreate();
		}

		mutexTake(LCD::g_mutex, MAX_DELAY);

		// update state
		LCD::g_modeStack->push(LCD::Enum);
		LCD::g_strings = strings;
		LCD::g_max = (int)max;
		LCD::g_callback = (void *)callback;

		// say that we have updated the state
		g_stateChanged = true;

		mutexGive(g_mutex);

		printf("modeStack->top() == %d\n", g_modeStack->top());
	}


	// this is all a HUGE MESS
	// since we dont have a std::function there is no easy way to pass around stateful llambda functions
	// TODO: maybe this will be better done with a struct functor?
	//       or maybe this is an inherent design flaw...
	extern Semaphore __WaitSemaphore;
	extern int __WaitValue;

	template<typename T>
	void __WaitCallback(T val)
	{
		__WaitValue = (int)val;
		if(semaphoreGive(__WaitSemaphore) != true)
		{
			// we were not able to give the semaphore back (wtf)
			// do nothing here for now...
			printf("UNABLE TO GIVE SEMAPHORE BACK\n\n\n\nFATAL ERROR!!!!!!\n\n\n\n");
		}

		return;
	}

	// Same as DisplayEnumOptions but blocks a thread
	template<typename Enum>
	Enum WaitDisplayEnumOptions(Enum max, const char **strings)
	{
		// we need to wait for the return of the enum
		__WaitSemaphore = semaphoreCreate();

		// call the function normally however with our callback
		// that signals when the function is done
		DisplayEnumOptions(max, strings, &__WaitCallback);

		// wait for value to be selected
		semaphoreTake(__WaitSemaphore, MAX_DELAY);

		// value has been selected, return it
		return (Enum)__WaitValue;
	}

	// Display a message on the screen for a certain delay
	// messages happen INSTANTLY - overriding the screen for the delay period
	// after which the last activity resumes (whether that be wait or otherwise)
	void DisplayMessage(int delay, const char *messageString);

	// Set the lcd update interval (for faster scrolling through values)
	void SetLcdUpdateInterval(int val);
}
