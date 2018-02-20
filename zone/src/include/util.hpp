#pragma once

#include "main.hpp"

template <typename T>
inline T min(T a, T b) {
    if (a < b)
        return a;
    return b;
}

template <typename T>
inline T max(T a, T b) {
    if (a > b)
        return a;
    return b;
}

template <typename T>
inline T abs(T val) {
    if (val < 0)
        return -val;

    return val;
}

template <typename T>
inline T clamp(T x, T min, T max) {
    if (x < min)
        return min;
    if (x > max)
        return max;

    return x;
}

template <class T, unsigned size = 10U>
class stack {
    T  data[size];
    T *top_ptr;

public:
    stack() {
        top_ptr = data;
    }

    T &top() {
        return *top_ptr;
    }

    T &pop() {
        return *(top_ptr--);
    }

    void push(T elem) {
        *(++top_ptr) = elem;
        return;
    }
};

class mutex {
    Mutex m;

public:
    mutex() {
        m = mutexCreate();
    }

    void lock() {
        mutexTake(m, -1);
    }

    void unlock() {
        mutexGive(m);
    }
};

class encoder {
    Encoder e;

    int last_time;
    int last_tick;

public:
    encoder() : last_time(0), last_tick(0) {
    }

    encoder(int top, int bot, bool reverse = false) : last_time(0), last_tick(0) {
        init(top, bot, reverse);
    }

    void init(int top, int bot, bool reverse = false) {
        e = encoderInit(top, bot, reverse);
        encoderReset(e);
    }

    ~encoder() {
        encoderShutdown(e);
    }

    void reset() {
        encoderReset(e);
    }

    int get_value(bool *success) {
        return get_tick(success);
    }

    int get_tick(bool *success) {
        int value = encoderGet(e);
        return value;
    }

    Encoder get_encoder() { return e; }
};

class ime {
    char ime_address;

public:
    ime() {}

    ime(char address) {
        ime_address = address;
        imeReset(ime_address);
    }

    void init(char address) {
        ime_address = address;
        imeReset(ime_address);
    }

    void reset() {
        imeReset(ime_address);
    }

    float get_value(bool *success) {
        int  value  = 0;
        bool result = imeGetVelocity(ime_address, &value);

        if (success != nullptr) {
            *success = result;
        }

        // high speed mode
        return value / 24.5;
    }

    // something something get revolutions instead
    int get_tick(bool *success) {
        int  value  = 0;
        bool result = imeGet(ime_address, &value);

        if (success != nullptr) *success = result;

        return value;
    }

    char get_encoder() { return ime_address; }
};

template <typename E>
class pid_helper_real {
    E enc;

    float p;
    float i;
    float d;
    float scale;
    float dt;

    float integral;
    float last_error;

    float requested;
    int   result_power;

    bool has_integral;

    mutex m;

public:
    pid_helper_real(E enc, float p, float i, float d, float scale) : enc(enc), p(p), i(i), d(d), scale(scale), has_integral(i != 0.0f) {}

    void  set_dt(int n) { dt = n; }
    void  set_requested(float delta_new) { requested += delta_new; }
    float get_requested() { return requested; }
    float get_error() { return last_error; }
    int   get_power() { return result_power; }

    float get_tick() { return enc.get_tick(nullptr); }

    E get_encoder() { return enc; }

    bool at_dest(int bounds) { return abs(requested - get_tick()) < bounds; }

    void reset() {
        m.lock();

        enc.reset();
        last_error = 0;
        integral   = 0;
        requested  = 0.0f;

        m.unlock();
    }

    void step() {
        m.lock();

        int   current_tick = get_tick();
        float error        = requested - current_tick;

        if (has_integral) {
            if (abs(error) < 50.0f) {
                integral = integral + error * dt;
            }
        } else {
            integral = 0.0f;
        }

        float derivative = error - last_error;
        last_error       = error;

        result_power = static_cast<int>((p * error) + (i * integral) + (d * derivative));

        m.unlock();
    }
};
