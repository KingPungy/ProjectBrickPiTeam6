#include "../include/controller.hpp"
#include "../include/helpmath.hpp"

classControl::classControl() {
    // fd = open("/dev/input/js0", O_RDONLY);
}

void classControl::process_input(input_event* input) {
    //printf("dereferencing\n");
    //input_event e = *input;
    //printf("dereferenced\n");

    //printf("id: %d, value: %d\n", input->id, input->value);

    switch (input->id - 16) {  // Buttons with values ranging from 0 to 1 (Bool)
        // A,B,X,Y Buttons
        case 0:
            a = input->value;
            break;
        case 1:
            b = input->value;
            break;
        case 2:
            x = input->value;
            break;
        case 3:
            y = input->value;
            break;
        // Buttons above the trigger
        case 4:
            lb = input->value;
            break;
        case 5:
            rb = input->value;
            break;
        // Buttons on the middle of the controller
        case 6:
            back = input->value;
            break;
        case 7:
            start = input->value;
            break;
        case 8:
            home = input->value;
            break;
        // Buttons of the joysticks
        case 9:
            lJb = input->value;
            break;
        case 10:
            rJb = input->value;
            break;

        // Analog values     With maps to -100 to 100
        // Left Joystick
        case 16:
            lJoyX = (int)map<float>(input->value, -32767, 32767, -100, 100);
            break;
        case 17:
            lJoyY = (int)map<float>(input->value - 255, -32767 - 255, 32767 - 255, 100, -100);
            break;
        // Right Joystick
        case 18:
            rJoyX = (int)map<float>(input->value, -32767, 32767, -100, 100);
            break;
        case 19:
            rJoyY = (int)map<float>(input->value - 255, -32767 - 255, 32767 - 255, 100, -100);
            break;
        // Triggers
        case 20:
            rTrig = (int)map<float>(input->value + 32767, 0, 2 * 32767, 0, 100);
            break;
        case 21:
            lTrig = (int)map<float>(input->value + 32767, 0, 2 * 32767, 0, 100);
            break;
        // D Pad leftRight and UpDown
        case 22: // dPad is considered a analog value but is has only 3 states. so we make it {-1, 0, 1}
            dLeftRight = input->value % 32766;
            break;
        case 23:
            dUpDown = -1 * (input->value % 32766);
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