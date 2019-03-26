#ifndef BRICKPI_HPP
#define BRICKPI_HPP

#include <signal.h>  // for catching exit signals
#include <stdio.h>   // for printf
#include <unistd.h>  // for usleep
#include <iostream>
#include "BrickPi3.cpp"

BrickPi3 BP;

sensor_light_t Light3;
void setup_motors_sensors();
void exit_signal_handler(int signo);

#endif