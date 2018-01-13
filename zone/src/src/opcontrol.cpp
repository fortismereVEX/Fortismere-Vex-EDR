#include "main.hpp"

#include "drive.hpp"
#include "lcd.hpp"

extern void auto_finish();

Encoder e;

void operatorControl() {

    lcd::suspendThread();

    // destroy autonomous thread
    auto_finish();

    while (true) {
        drive::run_frame();
        delay(1);
    }
}
