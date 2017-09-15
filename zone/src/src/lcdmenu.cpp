#include "../include/lcdmenu.h"
#include "../include/util.h"

// define lcd state variables
namespace LCD
{
	Mutex g_mutex;

	float *g_rtvalue;
	float g_rtinterval;
	bool g_rthasminmax;
	float g_rtmax;
	float g_rtmin;

	int g_interval;

	int g_messageWait;

	const char **g_strings;

	// static buffer for message
	char g_messageBuffer[16];
	const char *g_message;

	int g_max;

	void *g_callback;

	bool g_stateChanged;

	Stack<LCDMode> *g_modeStack;

	void LcdTask(void *param)
	{
		lcdInit(LCD_PORT);
		lcdClear(LCD_PORT);
		lcdSetBacklight(LCD_PORT, true);

		int currentValue = 0;
		int buttonsPressed = 0;
		g_stateChanged = false;

		//printf("\nCreating stack\n");

	 	Stack<LCDMode> __modestack;
		g_modeStack = &__modestack;

		//printf("\nPushing wait\n");

		// initialise a default mode
		__modestack.push(LCDMode::Wait);

		if(g_mutex == NULL)
		{
			g_mutex = mutexCreate();
		}

		Log::Print("\n\n\n\nLCD COMPLETE\n\n\n\n");

		while(true)
		{
			// take mutex once per loop (wait forever)
			mutexTake(g_mutex, MAX_DELAY);

			switch(g_modeStack->top())
			{
			case Wait:
				// dont do anything in a waiting state
				break;
			case Enum:
				{
					// check for state changes within this function
					// we dont want to reset these if it is just a message
					// TODO: is this the best way to do this?
					if(g_stateChanged == true)
					{
						// state was changed - reset "current" values
						printf("STATE_CHANGE!!!!!\n");
						currentValue = 0;
						buttonsPressed = 0;
						g_stateChanged = false;
					}

					printf("max: %d currentValue %d strings: '%s'\n", g_max + 1, currentValue, g_strings[abs(currentValue)]);
					lcdPrint(LCD_PORT, 1, "%s %d", g_strings[abs(currentValue)], currentValue);


					// get the last red buttons
					// value is a 3 bit integer 0 0 0
					//                          L C R
					buttonsPressed = lcdReadButtons(LCD_PORT);

					//printf("buttons is %d\n", buttonsPressed);

					if(buttonsPressed == 4) // right
					{
						// increment value
						//printf("INCREMENT\n");
						currentValue = Min(currentValue + 1, g_max);
						//printf("NEW VALUE: %d\n", currentValue);
					}
					else if(buttonsPressed == 1) // left
					{
						// decrement value

						// let the current value become negative (correcting it just results in more problems)
						// when accessing strings you need to get the abs() of the value
						//printf("DECREMENT\n");
						currentValue = Max(currentValue - 1, 0);
					}
					else if(buttonsPressed == 2) // center
					{
						// when the button is pressed twice in a row it is selected
						//lcdClear(LCD_PORT);

						// it is important to use abs because the currentValue could be negative!
						lcdPrint(LCD_PORT, 2, "selected");

						// update our state and the last state to the waiting state
						g_modeStack->pop();

						// release the mutex before calling
						// to prevent deadlock with functions that wish to display messages in their callbacks
						printf("Release mutex\n");
						mutexGive(g_mutex);

						printf("call function\n");
						// call the callback with the selected value
						((void(*)(int))g_callback)(abs(currentValue));

						printf("function return\ntake mutex\n");
						mutexTake(g_mutex, MAX_DELAY);
						printf("success\n");

						//continue;
					}
					break;
				}
			case Message:
				{
					if(g_message == nullptr)
					{
						g_modeStack->pop();
						break;
					}

					printf("MESSAGE\n");
					lcdClear(LCD_PORT);
					lcdPrint(LCD_PORT, 1, "%s", g_message);

					// wait for the delay interval
					delay(g_messageWait);
					g_message = NULL;

					g_modeStack->pop();

					break;
				}
			case RtUpdate:
				{
					if(g_rtvalue == nullptr)
					{
						g_modeStack->pop();
						break;
					}

					lcdPrint(LCD_PORT, 1, "RT: %f I: %f", *g_rtvalue, g_rtinterval);

					// get the last red buttons
					// value is a 3 bit integer 0 0 0
					//                          L C R
					buttonsPressed = lcdReadButtons(LCD_PORT);


					if(buttonsPressed == 4) // right
					{
						if(g_rthasminmax)
						{
							*g_rtvalue = Min(*g_rtvalue + g_rtinterval, g_rtmax);
						}
					}
					else if(buttonsPressed == 1) // left
					{
						if(g_rthasminmax)
						{
							*g_rtvalue = Max(*g_rtvalue - g_rtinterval, g_rtmin);
						}
					}
					else if(buttonsPressed == 2) // center
					{
						// we are now finished
						g_modeStack->pop();
						break;
					}

				}
			default:
				{
					lcdClear(LCD_PORT);
					lcdPrint(LCD_PORT, 0, "Complain to Josh...");
				}
			}

			// release mutex
			mutexGive(g_mutex);

			// wait for the specified interval - also allows other threads to work
			delay(g_interval);
		}
	}

	Semaphore __WaitSemaphore;
	int __WaitValue;

	void DisplayMessage(int delay, const char *messageString)
	{
		mutexTake(g_mutex, MAX_DELAY);

		Log::Print("DISPLAY MESSAGE\n");

		g_modeStack->push(LCDMode::Message);
		g_message = messageString;
		g_messageWait = delay;

		mutexGive(g_mutex);
	}

	void SetLcdUpdateInterval(int val)
	{
		mutexTake(g_mutex, MAX_DELAY);
		g_interval = val;
		mutexGive(g_mutex);
	}
}
