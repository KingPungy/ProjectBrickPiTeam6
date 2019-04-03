#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <fcntl.h>
#include <unistd.h>
#include <bitset>
#include <fstream>
#include <iostream>

struct js_event {
    uint32_t buttons;
    int32_t joyX;
    uint8_t joyY;
    uint8_t uselessStuff;
};

class classControl {
   public:
    bool a, b, x, y;
    bool lb, rb;
    bool home, back, start;
    bool lJb, rJb;
    int lJoyX, lJoyY, rJoyX, rJoyY;
    int rTrig, lTrig;
    int dLeftRight, dUpDown;
    int fd;
    struct js_event e;

    classControl();

    void update();

    void printInput();
};

#endif