#include "lcdmenu.h"
#include "util.h"

// define lcd state variables

Mutex LCD::g_mutex;

LCD::LCDMode LCD::g_mode;

void *LCD::g_value = nullptr;

int LCD::g_interval;

int LCD::g_messageWait;

const char **LCD::g_strings = nullptr;

extern const char *LCD::g_message = nullptr;

int LCD::g_max;

void *LCD::g_callback;

bool LCD::g_stateChanged;

void LCD::LcdTask(void *param)
{
	int currentValue = 0;
	int buttonsPressed = 0;

	LCD::LCDMode lastMode = LCD::Wait;

	while(true)
	{
		// take mutex once per loop (wait forever)
		mutexTake(LCD::g_mutex, MAX_DELAY);

		switch(LCD::g_mode)
		{
		case LCD::Wait:
			// dont do anything in a waiting state
			break;
		case LCD::Enum:
			{
				// check for state changes within this function
				// we dont want to reset these if it is just a message
				// TODO: is this the best way to do this?
				if(LCD::g_stateChanged)
				{
					// state was changed - reset "current" values
					currentValue = 0;
					buttonsPressed = 0;
				}

				// get the last red buttons
				// value is a 3 bit integer 0 0 0
				//                          L C R
				buttonsPressed = lcdReadButtons(uart1);

				if(buttonsPressed == 1) // right
				{
					// increment value
					currentValue = (currentValue + 1) % LCD::g_max;
				}
				else if(buttonsPressed == 4) // left
				{
					// decrement value

					// let the current value become negative (correcting it just results in more problems)
					// when accessing strings you need to get the abs() of the value
					currentValue = (currentValue - 1) % LCD::g_max);
				}
				else if(buttonsPressed == 2) // center
				{
					// when the button is pressed twice in a row it is selected
					lcdClear(uart1);

					// it is important to use abs because the currentValue could be negative!
					lcdPrint(uart1, 0, "%s %d", LCD::g_strings[abs(currentValue)], currentValue);
					lcdPrint(uart1, 1, "selected");

					// update our state and the last state to the waiting state
					LCD::g_mode = lastMode = LCD::Wait;


					// call the callback with the selected value
					((void(*)(int))callback)(abs(currentValue));

					continue;
				}

				// display the currently selected mode from the list
				lcdClear(uart1);
				lcdPrint(uart1, 0, "%s %d", LCD::g_strings[abs(currentValue)], currentValue);
				break;
			}
		case LCD::Message:
			{
				lcdClear(uart1);
				lcdPrint(uart1, 0, "%s", LCD::g_message);

				// wait for the delay interval
				delay(LCD::g_messageWait);
				LCD::g_mode = lastMode;
			}
		default:
			{
				lcdClear(uart1);
				lcdPrint(uart1, 0, "Complain to Josh...");
			}
		}


		// update the last mode (this is important for the message mode)
		lastMode = LCD::g_mode;


		// release mutex
		mutexGive(LCD::g_mutex);

		// wait for the specified interval - also allows other threads to work
		delay(LCD::g_interval);
	}
}

void DisplayMessage(int delay, const char *messageString)
{
	mutexTake(LCD::g_mutex);

	LCD::g_mode = LCD::Message;
	LCD::g_message = messageString;
	LCD::g_messageWait = delay;

	mutexGive(LCD::g_mutex);
}

void LCD::SetLcdUpdateInterval(int val)
{
	mutexTake(LCD::g_mutex);
	LCD::g_interval = val;
	mutexGive(LCD::g_mutex);
}
