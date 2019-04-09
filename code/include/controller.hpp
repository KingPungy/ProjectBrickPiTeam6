#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

//#include <fcntl.h>
#include <unistd.h>
//#include <bitset>
//#include <fstream>
//#include <iostream>


#define INPUT_STRUCT_SIZE 12

struct input_event
{
    int16_t value;
    uint8_t id;
};

class classControl {
   protected:
    // no name needed here really, it is a variable now
    union
    {
        uint8_t buf[INPUT_STRUCT_SIZE];
        struct
        {
            int16_t X1          = 0;
            int16_t Y1          = 0;
            int16_t X2          = 0;
            int16_t Y2          = 0;
            // say to the bools they are 1 bit wide instead of 8 bits for packing purposes
            bool du     : 1     = 0;
            bool dd     : 1     = 0;
            bool dl     : 1     = 0;
            bool dr     : 1     = 0;
            bool back   : 1     = 0;
            bool guide  : 1     = 0;
            bool start  : 1     = 0;
            bool TL     : 1     = 0;
            bool TR     : 1     = 0;
            bool A      : 1     = 0;
            bool B      : 1     = 0;
            bool X      : 1     = 0;
            bool Y      : 1     = 0;
            bool LB     : 1     = 0;
            bool RB     : 1     = 0;
            // put in this extra bit here so there is 16 instead of an akward 15
            bool extra  : 1     = 0;
            uint8_t LT          = 0;
            uint8_t RT          = 0;
        //__attribute__((packed)) means the compiler won't add padding
        } __attribute__((packed)) s;
    } __attribute__((packed)) u_input_data;

public:
    classControl();
    // fills u_input_data with input
    void process_input_controller_btn_all(void* input);
    void process_input_controller_btn_change(input_event* input);

    void printInput();

    // controller button and joystick getters
    const bool a() const;
    const bool b() const;
    const bool x() const;
    const bool y() const;
    const bool lb() const;
    const bool rb() const;
    const bool home() const;
    const bool back() const;
    const bool start() const;
    const bool lJb() const;
    const bool rJb() const;
    const float lJoyX() const;
    const float lJoyY() const;
    const float rJoyX() const;
    const float rJoyY() const;
    const float rTrig() const;
    const float lTrig() const;
    const bool dLeft() const;
    const bool dRight() const;
    const bool dUp() const;
    const bool dDown() const;
};

#endif