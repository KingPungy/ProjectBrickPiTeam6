#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <fcntl.h>
#include <unistd.h>
#include <bitset>
#include <fstream>
#include <iostream>

struct input_event
{
    int16_t value;
    uint8_t id;
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
    //int fd;
    //struct js_event e;

    classControl();

    void process_input(const input_event* input);

    void update();

    void printInput();
};

#endif