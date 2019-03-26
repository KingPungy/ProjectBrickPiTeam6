#include "controller.hpp"

classControl::classControl() { fd = open("/dev/input/js0", O_RDONLY); }

void classControl::update() {
    read(fd, &e, sizeof(e));
    a = e.buttons % 2;
    b = (e.buttons >> 1) % 2;
    x = (e.buttons >> 2) % 2;
    y = (e.buttons >> 3) % 2;

    lb = (e.buttons >> 4) % 2;
    rb = (e.buttons >> 5) % 2;

    home = (e.buttons >> 8) % 2;
    back = (e.buttons >> 6) % 2;
    start = (e.buttons >> 7) % 2;

    bjl = (e.buttons >> 9) % 2;
    bjr = (e.buttons >> 10) % 2;

    joyX = ((e.joyX - 128) / 127.0) * 100;
    joyY = ((-1 * (e.joyY - 128)) / 127.0) * 100;
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