#include "main.hpp"

#include "lcd.hpp"
#include "drive.hpp"

enum class enums
{
	wow,
	nice,
	meme,

	enums_max = meme,
};

char *enums_strings[] =
{
	"wow",
	"nice",
	"meme",
};

void enums_callback(enums val)
{
	printf("value recieved: %s [%d]\n", enums_strings[(int)val], val);
}

void operatorControl()
{
	//lcd::displayOptions(enums::enums_max, enums_strings, enums_callback);

	float val = 5.0f;
	float min = 0.0f;
	float max = 10.0f;

	while (true)
	{
		drive::run_frame();

		delay(1);
	}
}
