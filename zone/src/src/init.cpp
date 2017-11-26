#include "main.hpp"

#include "drive.hpp"
#include "lcd.hpp"

extern "C" {
void __libc_init_array();
}

void initializeIO() {
    watchdogInit();
}

// TEMPORARY
enum class autonomous_options {
    autonomous_blue_right = 0,
    autonomous_blue_left,
    autonomous_red_right,
    autonomous_red_left,

    autonomous_max = autonomous_red_left
};

const char *autonomous_options_strings[] = {
    "blue_right",
    "blue_left",
    "red_right",
    "red_left",
};

void autonomous_options_callback(autonomous_options result) {
    extern int g_autonomous;
    g_autonomous = (int)result;
}

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

    printf("====================\n");
    printf("init complete\n");
    printf("====================\n");

    lcd::displayOptions(autonomous_options::autonomous_max, autonomous_options_strings, &autonomous_options_callback);
}
