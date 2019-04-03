#include "../include/controller.hpp"

classControl::classControl() { fd = open("/dev/input/js0", O_RDONLY); }

void classControl::update() {
    read(fd, &e, sizeof(e));
    

    /*
        InputID-=16;

    switch (InputID)
    {   // Buttons
        case 0:
            a=e.value;
            break;
        case 1:
            b=e.value;
            break;
        case 2:
            x=e.value;
            break;            
        case 3:
            y=e.value;
            break;            
        case 4:
            lb=e.value;
            break;            
        case 5:
            rb=e.value;
            break;            
        case 6:
            back=e.value;
            break;            
        case 7:
            start=e.value;
            break;            
        case 8:
            home=e.value;
            break;            
        case 9:
            lJb=e.value;
            break;            
        case 10:
            rJb=e.value;
            break;            
        // Analog values
        case 16:
            lJoyX = e.value;
            break;
        case 17:
            lJoyY = e.value;
            break;
        case 18:
            rJoyX = e.value;
            break;
        case 19:
            rJoyY = e.value;
            break;
        case 20:
            rTrig = e.value;
            break;
        case 21:
            lTrig = e.value;
            break;
        case 22:
            dLeftRight = e.value;
            break;
        case 23:
            dUpDown = e.value;
            break;
            
    }
    */
}

void classControl::printInput() {
    std::cout << "A: " << a << "\tB:" << b << "\tX: " << x << "\tY: " << y
              << "\tLB: " << lb << "\tRB: " << rb << "\t(" << joyX << ", "
              << joyY << ")"
              << "\tHome: " << home << "\tBack: " << back
              << "\tStart: " << start << " BjL: " << bjl << "\tBjR: " << bjr
              << std::endl;
}

// int main() {
//     classControl controller;

//     while (true) {
//         controller.update();

//         controller.printInput();

//         usleep(10 * 1000);
//     }

//     return 0;
// }