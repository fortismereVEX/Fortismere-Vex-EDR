#include "main.hpp"

#include "lcd.hpp"

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
	printf("value recieved: %s [%d]", enums_strings[(int)val], val);
}

void operatorControl()
{

	lcd::displayOptions(enums::enums_max, enums_strings, enums_callback);

	float val = 5.0f;
	float min = 0.0f;
	float max = 10.0f;

	lcd::displayRealtime(&val, &min, &max);

	while (true)
	{
		lcd::displayMessage("message wow");
		delay(2000);

	}
}
