#pragma once
#include <main.hpp>

#include "util.hpp"

#include <stdarg.h>

class lcd {
    enum state {
        wait,
        message,
        options,
        realtime,
    };

    enum realtimeType {
        None,
        Int,
        Float,
        Bool,
    };

    static TaskHandle   lcd_task_handle;
    static stack<state> state_stack;

    static char *message_string;
    static int   message_delay;
    static bool  message_allocated;

    static mutex lcd_mutex;

    static int    option_value;
    static int    option_min;
    static int    option_max;
    static char **option_strings;
    static void (*option_callback)(int);

    static void (*realtime_callback)(void *, int);
    static void *       realtime_val;
    static void *       realtime_max;
    static void *       realtime_min;
    static realtimeType realtime_type;

public:
    static void initializeIO() {
    }

    static void lcdtask(void *) {

        while (true) {
            lcd_mutex.lock();

            switch (state_stack.top()) {
            case wait: {
                // implement a menu for this
                lcdPrint(uart1, 2, "waiting...");
                break;
            }
            case message: {
                lcdPrint(uart1, 1, message_string);
                delay(1000);
                state_stack.pop();
                break;
            }
            case options: {
                option_value = 0;
                // intentionally block here
                while (true) {
                    int buttons = lcdReadButtons(uart1);
                    if (buttons == 4) {
                        // right = increment
                        option_value = min(option_value + 1, option_max);
                    } else if (buttons == 1) {
                        // left = decrement
                        option_value = max(option_value - 1, option_min);
                    } else if (buttons == 2) {
                        // center = select
                        lcdPrint(uart1, 2, "selected...");
                        delay(100);

                        // pop the state and return the mutex
                        // to prevent deadlock if the callback
                        // calls back into lcd code
                        state_stack.pop();

                        lcd_mutex.unlock();
                        option_callback(option_value);

                        // relock for the code that follows
                        lcd_mutex.lock();

                        break;
                    }
                    if (option_value == option_max) {
                        lcdPrint(uart1, 1, "\xF7 %s", option_strings[option_value]);
                    } else if (option_value == option_min) {
                        lcdPrint(uart1, 1, "%s \xF6", option_strings[option_value]);
                    } else {
                        lcdPrint(uart1, 1, "\xF7 %s \xF6", option_strings[option_value]);
                    }
                    // display the current value

                    delay(200);
                }
                break;
            }
            case realtime: {
                // intentional block
                ::printf("realtime\n");
                while (true) {
                    int buttons = lcdReadButtons(uart1);

                    if (buttons == 4) {
                        // right = increment
                        switch (realtime_type) {
                        case Float: {
                            float *val = static_cast<float *>(realtime_val);
                            float *max = static_cast<float *>(realtime_max);
                            *val       = min(*val + 0.1f, *max);
                            break;
                        }
                        case Int: {
                            int *val = static_cast<int *>(realtime_val);
                            int *max = static_cast<int *>(realtime_max);
                            *val     = min(*val + 1, *max);
                            break;
                        }
                        case Bool: {
                            bool *val = static_cast<bool *>(realtime_val);
                            *val      = !(*val);
                            break;
                        }
                        case None: {
                            lcdPrint(uart1, 1, "type is `None`");
                            lcdPrint(uart1, 2, "This is a bug.");
                            delay(1000);
                            break;
                        }
                        }
                    } else if (buttons == 1) {
                        // left = decrement
                        switch (realtime_type) {
                        case Float: {
                            float *val = static_cast<float *>(realtime_val);
                            float *min = static_cast<float *>(realtime_min);
                            *val       = max(*val - 0.1f, *min);
                            break;
                        }
                        case Int: {
                            int *val = static_cast<int *>(realtime_val);
                            int *min = static_cast<int *>(realtime_min);
                            *val     = max(*val - 1, *min);
                            break;
                        }
                        case Bool: {
                            bool *val = static_cast<bool *>(realtime_val);
                            *val      = !(*val);
                            break;
                        }
                        case None: {
                            lcdPrint(uart1, 1, "realtime_type is None");
                            delay(1000);
                            break;
                        }
                        }
                    } else if (buttons == 2) {
                        // we are done changing
                        state_stack.pop();
                        lcdPrint(uart1, 2, "finished...");
                        delay(100);
                        break;
                    }

                    switch (realtime_type) {
                    case Float: {
                        lcdPrint(uart1, 1, "%f", *static_cast<float *>(realtime_val));
                        break;
                    }
                    case Int: {
                        lcdPrint(uart1, 1, "%d", *static_cast<int *>(realtime_val));
                        break;
                    }
                    case Bool: {
                        lcdPrint(uart1, 1, "%s", *static_cast<bool *>(realtime_val) ? "true" : "false");
                        break;
                    }
                    case None: {
                        lcdPrint(uart1, 1, "realtime_type is None");
                        delay(1000);
                        break;
                    }
                    }

                    delay(200);
                }
                break;
            }
            }
            lcd_mutex.unlock();
            delay(1);
        }
    }

    static void initialize() {
        // open uart port
        lcdInit(uart1);
        lcdClear(uart1);
        lcdSetBacklight(uart1, true);

        // initialise the statestack
        state_stack.push(state::wait);

        // create lcd task
        lcd_task_handle = taskCreate(&lcdtask, TASK_DEFAULT_STACK_SIZE, nullptr, TASK_PRIORITY_DEFAULT);
    }

    static void suspendThread() {
        taskSuspend(lcd_task_handle);
        lcd_mutex.unlock(); // make sure to unlock the mutex so there are no deadlocks
    }
    static void resumeThread() {
        taskResume(lcd_task_handle);
    }

    static void displayMessage(char *message) {
        lcd_mutex.lock();
        state_stack.push(state::message);
        message_string    = message;
        message_allocated = false;
        lcd_mutex.unlock();
    }

    static void printf(const char *format, ...) {
        lcd_mutex.lock();
        state_stack.push(state::message);
        message_allocated = true;
        message_string    = (char *)malloc(15);

        va_list vlist;
        va_start(vlist, format);
        extern int vsnprintf(char *out, size_t size, const char *fmt, va_list args);
        vsnprintf(message_string, 15, format, vlist);
        va_end(vlist);

        ::printf("%s\n", message_string);

        lcd_mutex.unlock();
    }

    template <typename T>
    static void displayOptions(T max, char **strings, void (*callback)(T)) {
        lcd_mutex.lock();
        state_stack.push(state::options);
        option_min      = 0;
        option_max      = (int)max;
        option_strings  = strings;
        option_callback = (void (*)(int))callback;
        lcd_mutex.unlock();
    }

    template <typename T>
    static void displayRealtime(T *val, T *min, T *max);
};

template <>
inline void lcd::displayRealtime<float>(float *val, float *min, float *max) {
    lcd_mutex.lock();
    state_stack.push(state::realtime);
    realtime_val  = (void *)val;
    realtime_min  = (void *)min;
    realtime_max  = (void *)max;
    realtime_type = realtimeType::Float;
    lcd_mutex.unlock();
}

template <>
inline void lcd::displayRealtime<int>(int *val, int *min, int *max) {
    lcd_mutex.lock();
    state_stack.push(state::realtime);
    realtime_val  = (void *)val;
    realtime_min  = (void *)min;
    realtime_max  = (void *)max;
    realtime_type = realtimeType::Int;
    lcd_mutex.unlock();
}

template <>
inline void lcd::displayRealtime<bool>(bool *val, bool *min, bool *max) {
    lcd_mutex.lock();
    state_stack.push(state::realtime);
    realtime_val  = (void *)val;
    realtime_min  = (void *)min;
    realtime_max  = (void *)max;
    realtime_type = realtimeType::Bool;
    lcd_mutex.unlock();
}
