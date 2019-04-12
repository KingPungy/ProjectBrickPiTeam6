#include "../include/controller.hpp"

#include "../include/helpmath.hpp"
#include <cstdio>
#include <cstring>
#include <climits>

classControl::classControl()
{
    
}

void classControl::process_input_controller_btn_all(void* input) {
    // copies all the input data to the input struct
    memcpy(u_input_data.buf, input, INPUT_STRUCT_SIZE);
}

void classControl::process_input_controller_btn_change(void* input) {
    input_event* ie = (input_event*)input;
    // evaluates input struct pointer and sets the correct variable in the input data struct
    switch (ie->id - 16) {
        // Buttons with values ranging from 0 to 1 (Bool)
        // A,B,X,Y Buttons
        case 0: u_input_data.s.A        = (bool)ie->value; break;
        case 1: u_input_data.s.B        = (bool)ie->value; break;
        case 2: u_input_data.s.X        = (bool)ie->value; break;
        case 3: u_input_data.s.Y        = (bool)ie->value; break;
        // Buttons above the trigger
        case 4: u_input_data.s.LB       = (bool)ie->value; break;
        case 5: u_input_data.s.RB       = (bool)ie->value; break;
        // Buttons on the middle of the controller
        case 6: u_input_data.s.back     = (bool)ie->value; break;
        case 7: u_input_data.s.start    = (bool)ie->value; break;
        case 8: u_input_data.s.guide    = (bool)ie->value; break;
        // Buttons of the joysticks
        case 9: u_input_data.s.TL       = (bool)ie->value; break;
        case 10: u_input_data.s.TR      = (bool)ie->value; break;

        // Analog values
        // Left Joystick
        case 16: u_input_data.s.X1      = (int16_t)ie->value; break;
        case 17: u_input_data.s.Y1      = (int16_t)ie->value; break;
        // Right Joystick
        case 18: u_input_data.s.X2      = (int16_t)ie->value; break;
        case 19: u_input_data.s.Y2      = (int16_t)ie->value; break;
        // Triggers
        case 20: u_input_data.s.RT      = (uint8_t)ie->value; break;
        case 21: u_input_data.s.LT      = (uint8_t)ie->value; break;
        // D Pad leftRight and UpDown
        case 22: // dPad is NOT considered analog
            //dLeftRight = ie->value % 32766;
            // u_input_data.s.du
            // u_input_data.s.dd
            break;
        case 23:
            //dUpDown = -1 * (ie->value % 32766);
            // u_input_data.s.dl
            // u_input_data.s.dr
            break;
    }
}

void classControl::printInput() { // Prints all button and axis values of the controller
    printf(
        "A: %d B: %d X: %d Y: %d LB: %d RB: %d Back: %d Start: %d Home: "
        "%d LeftJoyButton: %d RightJoyButton: %d\n",
        a(), b(), x(), y(), lb(), rb(), back(), start(), home(), lJb(), rJb());
    printf(
        "LeftJoyX: %6f LeftJoyY: %6f RightJoyX: %6f RightJoyY: %6f lTrigger: "
        "%6f rTrigger: %6f dleft: %6d dRight: %6d dUp: %6d Down: %6d\n", 
        lJoyX(), lJoyY(), rJoyX(), rJoyY(), lTrig(), rTrig(), dLeft(), dRight(), dUp(), dDown());
}
// Makes a function that returns its valu
const bool classControl::a() const {
    return u_input_data.s.A;
}
const bool classControl::b() const {
    return u_input_data.s.B;
}
const bool classControl::x() const {
    return u_input_data.s.X;
}
const bool classControl::y() const {
    return u_input_data.s.Y;
}
const bool classControl::lb() const {
    return u_input_data.s.LB;
}
const bool classControl::rb() const {
    return u_input_data.s.RB;
}
const bool classControl::home() const {
    return u_input_data.s.guide;
}
const bool classControl::back() const {
    return u_input_data.s.back;
}
const bool classControl::start() const {
    return u_input_data.s.start;
}
const bool classControl::lJb() const {
    return u_input_data.s.TL;
}
const bool classControl::rJb() const {
    return u_input_data.s.TR;
}

// map anologue joystick values from -100 to 100
const float classControl::lJoyX() const {
    return map<float>(u_input_data.s.X1, SHRT_MIN, SHRT_MAX, -100, 100);
}
const float classControl::lJoyY() const {
    return map<float>(u_input_data.s.Y1, SHRT_MIN, SHRT_MAX, -100, 100);
}
const float classControl::rJoyX() const {
    return map<float>(u_input_data.s.X2, SHRT_MIN, SHRT_MAX, -100, 100);
}
const float classControl::rJoyY() const {
    return map<float>(u_input_data.s.Y2, SHRT_MIN, SHRT_MAX, -100, 100);
}

// map anologue trigger values from 0 to 100
const float classControl::rTrig() const {
    return map<float>(u_input_data.s.RT, 0, UCHAR_MAX, 0, 100);
}
const float classControl::lTrig() const {
    return map<float>(u_input_data.s.LT, 0, UCHAR_MAX, 0, 100);
}

const bool classControl::dLeft() const {
    return u_input_data.s.dl;
}
const bool classControl::dRight() const {
    return u_input_data.s.dr;
}
const bool classControl::dUp() const {
    return u_input_data.s.du;
}
const bool classControl::dDown() const {
    return u_input_data.s.dd;
}