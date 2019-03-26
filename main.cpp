/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering

*/
#include <signal.h>      // for catching exit signals
#include <stdio.h>       // for printf
#include <unistd.h>      // for usleep
#include "BrickPi3.cpp"  // for BrickPi3
#include "controller.hpp"

BrickPi3 BP;

void exit_signal_handler(int signo);

int main() {
    signal(SIGINT, exit_signal_handler);  // register exit for Ctrl+C

    BP.detect();
    classControl controller;

    while (true) {
        controller.update();
        // Use the encoder value from motor A to control motors B, C, and D
        // BP.set_motor_dps(PORT_C, PositionA);
        // if (controller.joyX > 0) {
        //     BP.set_motor_dps(PORT_B, controller.joyY * 4 + controller.joyX *
        //     4); BP.set_motor_dps(PORT_C, controller.joyY * 4);
        // } else if (controller.joyX < 0) {
        //     BP.set_motor_dps(PORT_B, controller.joyY * 4);
        //     BP.set_motor_dps(PORT_C,
        //                      controller.joyY * 4 + abs(controller.joyX * 4));
        // }
        // else {
        BP.set_motor_dps(PORT_B, controller.joyY * 4 + controller.joyX * 4);
        BP.set_motor_dps(PORT_C, controller.joyY * 4 - controller.joyX * 4);
        // }BP.set_motor_dps(PORT_B, controller.joyY * 4);
        BP.set_motor_dps(PORT_C, controller.joyY * 4 + controller.joyX * 4);

        // usleep(10 * 1000);
    }
    BP.set_motor_dps(PORT_C, 0);
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo) {
    if (signo == SIGINT) {
        BP.reset_all();  // Reset everything so there are no run-away motors
        exit(-2);
    }
}