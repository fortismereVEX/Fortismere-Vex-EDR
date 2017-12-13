#include "main.hpp"

#include "drive.hpp"
#include "lcd.hpp"

extern "C" {
void __libc_init_array();
}

void initializeIO() {
    watchdogInit();
}

// TOOD: header and static class
extern void auto_initialize();

void initialize() {
    __libc_init_array();

    printf("=========================\n");
    printf("Fortismere Vex\n");
    printf("=========================\n");

    printf("\n\n\n");

    printf("=====================\n");
    printf("running init code\n");
    printf("=====================\n");

    printf("=> lcd\n");
    lcd::initialize();

    printf("=> drive\n");
    drive::initialize();

    print("=> auto\n");
    auto_initialize();

    printf("====================\n");
    printf("init complete\n");
    printf("====================\n");
}
