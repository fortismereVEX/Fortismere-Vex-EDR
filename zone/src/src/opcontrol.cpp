#include "main.hpp"

#include "drive.hpp"
#include "lcd.hpp"


void operatorControl() {
    while (true) {
        drive::run_frame();

        delay(1);
    }
}
