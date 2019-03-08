/*
 *  https://www.dexterindustries.com/BrickPi/
 *  https://github.com/DexterInd/BrickPi3
 *
 *  Copyright (c) 2017 Dexter Industries
 *  Released under the MIT license (http://choosealicense.com/licenses/mit/).
 *  For more information, see https://github.com/DexterInd/BrickPi3/blob/master/LICENSE.md
 *
 *  This code is an example for reading the encoders of motors connected to the BrickPi3.
 *
 *  Hardware: Connect NXT sensors to the sensor ports. Color sensor to PORT_1. Ultrasonic sensor to PORT_2. Light sensor to PORT_3. Touch sensor to PORT_4 (EV3 or NXT touch sensor).
 *
 *  Results:  When you run this program, you should see the values for each sensor.
 *
 *  Example compile command:
 *    g++ -o program "sensors_nxt.c"
 *  Example run command:
 *    sudo ./program
 *
 */

#include "BrickPi3.cpp" // for BrickPi3
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>

BrickPi3 BP;

void exit_signal_handler(int signo);

int main(){
  signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
  int error;
  
  BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
  // Motors
  BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
  BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
  
  // Sensors
  BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
  sensor_light_t Light3;
  // BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);
  // sensor_ultrasonic_t Ultrasonic4;

  for (unsigned int i = 0; i < 5000; i++) {
    BP.get_sensor(PORT_3, &Light3)
    std::cout << Light3.reflected << std::endl;
    if (Light3.reflected > 1800) {
      BP.set_motor_dps(PORT_C, 0);
      BP.set_motor_dps(PORT_B, 200);
      
      //linkerwiel
    } else {
      BP.set_motor_dps(PORT_B, 0);
      BP.set_motor_dps(PORT_C, 200);
      //rechterwiel
    }
    usleep(10000);
  }

  BP.set_motor_dps(PORT_B, 0);
  BP.set_motor_dps(PORT_C, 0);

  // for (unsigned int i = 0; i < 10000; i++) {
  //   error = 0;
    
  //   if(BP.get_sensor(PORT_3, &Light3)){
  //     error++;
  //   }else{
  //     std::cout <<  "Light sensor (S3): reflected: " << Light3.reflected << std::endl;
  //   }
    
  //   usleep(2000);
  // }
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}
