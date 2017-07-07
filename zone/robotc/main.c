

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"


#include "lcd.c"




//
//
// Main Code
//
//
//



void pre_auton()
{

  bStopTasksBetweenModes = true;

}


task autonomous()
{
  AutonomousCodePlaceholderForTesting();
}


task usercontrol()
{
  while (true)
  {
    UserControlCodePlaceholderForTesting();
  }
}
