/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering


*/
#include "BrickPi.hpp"

int main() {
    signal(SIGINT,
           exit_signal_handler);  // register the exit function for Ctrl+C
    int error;

    setup_motors_sensors();

    for (unsigned int i = 0; i < 5000; i++) {
        BP.get_sensor(PORT_3, &Light3);
        std::cout << Light3.reflected << std::endl;
        if (Light3.reflected > 1800) {
            BP.set_motor_dps(PORT_C, 0);
            BP.set_motor_dps(PORT_B, 400);

            // linkerwiel
        } else {
            BP.set_motor_dps(PORT_B, 0);
            BP.set_motor_dps(PORT_C, 400);

            // rechterwiel
        }

        usleep(5 * 1000);  // 1000 = 1 millisecond
    }

    BP.set_motor_dps(PORT_B, 0);
    BP.set_motor_dps(PORT_C, 0);
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo) {
    if (signo == SIGINT) {
        BP.reset_all();  // Reset everything so there are no run-away motors
        exit(-2);
    }
}