#include <main.hpp>

class lcd
{
public:
	static void initializeIO()
	{
	}

	static void lcdtask(void *)
	{
		printf("lcd code wow \n");

		while(true)
		{
			lcdPrint(uart1, 2, "wow nice meme");
			delay(1);
		}
	}

	static void initialize()
	{
		// open uart port
		lcdInit(uart1);
		lcdClear(uart1);
		lcdSetBacklight(uart1, true);
		// create lcd task
		taskCreate(&lcdtask, TASK_DEFAULT_STACK_SIZE, nullptr, TASK_PRIORITY_DEFAULT);
	}
};
