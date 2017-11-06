#include "lcd.hpp"

stack<lcd::state> lcd::state_stack;
char *lcd::message_string;
bool lcd::message_allocated = false;
mutex lcd::lcd_mutex;

int lcd::option_value;
int lcd::option_min;
int lcd::option_max;
char **lcd::option_strings;
void(*lcd::option_callback)(int);

void(*lcd::realtime_callback)(void *, int);
void *lcd::realtime_val;
void *lcd::realtime_max;
void *lcd::realtime_min;
lcd::realtimeType lcd::realtime_type;
