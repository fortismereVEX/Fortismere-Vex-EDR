#pragma once
#include <main.h>

class demo {
public:
    enum mode {
        mode_read,
        mode_write,
    };

private:
    char *   buffer;
    unsigned size;
    unsigned pos;
    mode     current_mode;

    void resize(unsigned additional) {
        if (current_mode != mode_write) {
            printf("trying to resize the buffer in read mode...\nThis is a code bug\n\n");
            lcd::displayMessage("r: resize in read");
        }

        size += additional;
        buffer = (char *)realloc(buffer, size);
    }

public:
    demo() : buffer(nullptr), size(256), pos(0) {
        buffer = (char *)malloc(size);
    }

    void switch_mode(mode m) {
        if (mode == mode_read) {
            current_mode = mode_read;
            pos          = 0;
        } else if (mode == mode_write) {
            current_mode = mode_write;
            pos          = 0;
        }
    }

    void write(char type, char value) {
        // write 1 byte for the type followed by a byte for the value
        // this may not be required and we could think about writing nibbles
        // for the type or for both
        if (pos + 2 >= size) {
            resize(64);
        }

        buffer[pos++] = type;
        buffer[pos++] = value;
    }

    bool save_to_disk(const char *filename) {
        if (current_mode != mode_write) {
            printf("trying to write a file in read mode\n");
            lcd::displayMessage("w: write in read");
        }

        // write a trailing 00 00 pair
        write(0, 0);

        FILE *f = fopen(filename, "w");

        if (f == nullptr) {
            lcd::displayMessage("w: unable to write to file");
            return false;
        }

        int wrote = fwrite(buffer, 1, size, f);

        if (wrote < size) {
            lcd::displayMessage("w: unable to write all bytes of file");
        }

        fclose(f);
    }

    bool read_from_disk(const char *filename) {
        if (current_mode != mode_read) {
            printf("trying to read a file in write mode\n");
            lcd::displayMessage("r: read in write");
        }

        FILE *f = fopen(filename, "r");

        if (f == nullptr) {
            lcd::displayMessage("r: unable to read file");
        }

        fseek(f, 0, SEEK_END);
        size = ftell(f);

        // rewind
        fseek(f, 0, SEEK_SET);

        int read = fread(buffer, 1, size, f);

        if (read != size) {
            printf("unable to read whole buffer\n");
        }
    }

    void read(char *type, char *byte) {
        type = &buffer[++position];
        byte = &buffer[++position];
    }
};