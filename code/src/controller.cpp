#include "../include/controller.hpp"
#include "../include/helpmath.hpp"

classControl::classControl() {
    // fd = open("/dev/input/js0", O_RDONLY);
}

void classControl::process_input(input_event* input) {
    input_event e = *input;

    switch (e.id - 16) {  // Buttons with values ranging from 0 to 1 (Bool)
        // A,B,X,Y Buttons
        case 0:
            a = e.value;
            break;
        case 1:
            b = e.value;
            break;
        case 2:
            x = e.value;
            break;
        case 3:
            y = e.value;
            break;
        // Buttons above the trigger
        case 4:
            lb = e.value;
            break;
        case 5:
            rb = e.value;
            break;
        // Buttons on the middle of the controller
        case 6:
            back = e.value;
            break;
        case 7:
            start = e.value;
            break;
        case 8:
            home = e.value;
            break;
        // Buttons of the joysticks
        case 9:
            lJb = e.value;
            break;
        case 10:
            rJb = e.value;
            break;

        // Analog values     With maps to -100 to 100
        // Left Joystick
        case 16:
            lJoyX = (int)map<float>(e.value, -32767, 32767, -100, 100);
            break;
        case 17:
            lJoyY = (int)map<float>(e.value - 255, -32767 - 255, 32767 - 255, 100, -100);
            break;
        // Right Joystick
        case 18:
            rJoyX = (int)map<float>(e.value, -32767, 32767, -100, 100);
            break;
        case 19:
            rJoyY = (int)map<float>(e.value - 255, -32767 - 255, 32767 - 255, 100, -100);
            break;
        // Triggers
        case 20:
            rTrig = (int)map<float>(e.value + 32767, 0, 2 * 32767, 0, 100);
            break;
        case 21:
            lTrig = (int)map<float>(e.value + 32767, 0, 2 * 32767, 0, 100);
            break;
        // D Pad leftRight and UpDown
        case 22: // dPad is considered a analog value but is has only 3 states. so we make it {-1, 0, 1}
            dLeftRight = e.value % 32766;
            break;
        case 23:
            dUpDown = -1 * (e.value % 32766);
            break;
    }
}

void classControl::update() {
    // read(fd, &e, sizeof(e));
    // process_input(&e);
    printInput();
}

void classControl::printInput() { // Prints all button and axis valuues of the controller
    printf(
        "\e[2AA: %d B: %d X: %d Y: %d LB: %d RB: %d Back: %d Start: %d Home: "
        "%d LeftJoyButton: %d RightJoyButton: %d\n",
        a, b, x, y, lb, rb, back, start, home, lJb, rJb);
    printf(
        "LeftJoyX: %6d LeftJoyY: %6d RightJoyX: %6d RightJoyY: %6d lTrigger: "
        "%6d rTrigger: %6d dleftRight: %6d dUpDown: %6d\n",
        lJoyX, lJoyY, rJoyX, rJoyY, lTrig, rTrig, dLeftRight, dUpDown);
}