#ifndef BRICKPI_HPP
#define BRICKPI_HPP

#include "BluetoothSocket.h" // for bluetooth control
#include <signal.h>  // for catching exit signals
#include <stdio.h>   // for printf
#include <unistd.h>  // for usleep
#include <iostream>
#include "BrickPi3.cpp"

BrickPi3 BP;

void setup_motors_sensors();
void exit_signal_handler(int signo);

#endif
