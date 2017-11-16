#include "main.hpp"

#include "drive.hpp"
#include "lcd.hpp"


extern "C" {
void __libc_init_array();
}

void initializeIO() {
    watchdogInit();
}

void initialize() {
    delay(1000);
    __libc_init_array();

    printf("=========================\n");
    printf("Fortismere Vex\n");
    printf("=========================\n");

    printf("\n\n\n");
    delay(500);

    printf("====================\n");
    printf("running init code\n");
    printf("=====================\n");

    lcd::initialize();
    drive::initialize();
}
