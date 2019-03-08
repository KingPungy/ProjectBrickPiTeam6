#include "BrickPi3.cpp"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

BrickPi3 BP;

void exit_signal_handler(int signo);

int main() {
    signal(SIGINT, exit_signal_handler);

    BP.detect();

    //reset the enocders
    BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));
    BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
    BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));

    BP.set_motor_position(PORT_B, 360*60);

}

void exit_signal_handler(int signo) {
    if (signo == SIGINT) {
        BP.reset_all();     // reset everytihing so there are no run-away motors
        exit_signal_handler(-2);
    }
}