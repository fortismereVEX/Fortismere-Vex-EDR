#include "main.hpp"

#include "lcd.hpp"
#include "drive.hpp"

extern "C"
{
// allow us to use the c++ static initialisation
void __libc_init_array();
}


void initializeIO()
{
	watchdogInit();
}

void initialize()
{
	__libc_init_array();

	lcd::initialize();
	drive::initialize();
}
