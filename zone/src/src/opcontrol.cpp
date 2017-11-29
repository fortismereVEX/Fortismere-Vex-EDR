#include "main.hpp"

#include "drive.hpp"
#include "lcd.hpp"

void operatorControl() {

    lcd::suspendThread();

    while (true) {
        drive::run_frame();

        delay(1);
    }
}
