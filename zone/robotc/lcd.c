//
//
// LCD Library
//
//

// this file defines a common interface to display options on the lcd display

typedef enum k_lcd_mode
{
	LCD_Wait,
	LCD_Enum,
	LCD_RtUpdate,
	LCD_Message,
} lcd_mode_t;

// define some state for the lcd

// state access mutex - important for syncronisation
TSemaphore g_mutex;

// semaphore for the wait semaphore
TSemaphore __WaitSemaphore;

// current lcd mode
lcd_mode_t g_mode;

// realtime update value
void *g_value;

// update interval
int g_interval;

int g_messageWait;

// "translations" for enum types
char **g_strings = NULL;

// message string data
char *g_message = NULL;

// max value for enums
int g_max;

// callback for value changes
void *g_callback;

bool g_stateChanged;

void Lcd_Init()
{
	semaphoreInitialize(g_mutex);
	semaphoreInitialize(__WaitSemaphore);
}

// this is the global task for the lcd menu
void Lcd_Task(void *param)
{
	int currentValue = 0;
	int buttonsPressed = 0;

	lcd_mode_t lastMode = LCD_Wait;

	while(true)
	{
		// take mutex once per loop (wait forever)
		semaphoreLock(g_mutex);

		clearLCDLine(0);
		clearLCDLine(1);

		switch(g_mode)
		{
		case LCD_Wait:
			// dont do anything in a waiting state
			break;
		case LCD_Enum:
			{
				// check for state changes within this function
				// we dont want to reset these if it is just a message
				// TODO: is this the best way to do this?
				if(g_stateChanged)
				{
					// state was changed - reset "current" values
					currentValue = 0;
					buttonsPressed = 0;
				}

				// get the last red buttons
				// value is a 3 bit integer 0 0 0
				//                          L C R
				buttonsPressed = nLCDButtons;

				if(buttonsPressed == 1) // right
				{
					// increment value
					currentValue = (currentValue + 1) % g_max;
				}
				else if(buttonsPressed == 4) // left
				{
					// decrement value

					// let the current value become negative (correcting it just results in more problems)
					// when accessing strings you need to get the abs() of the value
					currentValue = (currentValue - 1) % g_max;
				}
				else if(buttonsPressed == 2) // center
				{
					// when the button is pressed twice in a row it is selected

					string valueString;
					string newValue =  g_strings[abs(currentValue)];
					sprintf(valueString, "%s %d", &g_strings[0][abs(currentValue)], currentValue);

					// it is important to use abs because the currentValue could be negative!
					displayLCDString(0, 0, valueString);
					displayLCDString(1, 0, "selected");

					// update our state and the last state to the waiting state
					g_mode = lastMode = LCD_Wait;

					// call the callback with the selected value
					((void(*)(int))g_callback)(abs(currentValue));

					continue;
				}

				// display the currently selected mode from the list

				string valueString;
				string newValue =  g_strings[abs(currentValue)];
				sprintf(valueString, "%s %d", newValue, currentValue);

				displayLCDString(0, 0, valueString);
				break;
			}
		case LCD_Message:
			{
				clearLCDLine(0);
				displayLCDCenteredString(g_message);

				// wait for the delay interval
				sleep(g_messageWait);
				g_message = NULL;
				g_mode = lastMode;
			}
		default:
			{
				clearLCDLine(0);
				displayLCDCenteredString("Complain to Josh...");
			}
		}


		// update the last mode (this is important for the message mode)
		lastMode = g_mode;


		// release mutex
		mutexGive(g_mutex);

		// wait for the specified interval - also allows other threads to work
		sleep(g_interval);
	}
}

void Lcd_DisplayEnumOptions(int max, const char **strings, void(*callback)(int val))
{
	semaphoreLock(g_mutex);

	// update state
	g_mode = Enum;
	g_strings = strings;
	g_max = (int)max;
	g_callback = (void *)callback;

	// say that we have updated the state
	g_stateChanged = true;

	semaphoreUnlock(g_mutex);
}


// this is all a HUGE MESS
// since we dont have a std::function there is no easy way to pass around stateful llambda functions
// TODO: maybe this will be better done with a struct functor?
//       or maybe this is an inherent design flaw...
int __WaitValue;

void __Lcd_WaitCallback(int val)
{
	__WaitValue = val;

	return;
}

// Same as DisplayEnumOptions but blocks a thread
int Lcd_WaitDisplayEnumOptions(int max, const char **strings)
{
	// call the function normally however with our callback
	// that signals when the function is done
	LCD_DisplayEnumOptions(max, strings, &__Lcd_WaitCallback);

	// attempt to lock the mutex as a method of waiting for the callback to return
	semaphoreLock(g_mutex);
	semaphoreUnlock(g_mutex);

	return (int)__WaitValue;
}

// Display a message on the screen for a certain delay
// messages happen INSTANTLY - overriding the screen for the delay period
// after which the last activity resumes (whether that be wait or otherwise)
void LCD_DisplayMessage(int delay, const char *messageString)
{
	semaphoreLock(g_mutex);

	g_mode = Message;
	g_message = messageString;
	g_messageWait = delay;

	semaphoreUnlock(g_mutex);
}

// Set the lcd update interval (for faster scrolling through values)
void SetLcdUpdateInterval(int val)
{
	semaphoreLock(g_mutex, MAX_DELAY);
	g_interval = val;
	semaphoreUnlock(g_mutex);
}
