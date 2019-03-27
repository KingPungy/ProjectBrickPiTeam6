
#include "../include/brick_io.hpp"
#include <unistd.h>  // for usleep
#include <iostream>

IO::IO() {
    BP.detect();

    BP.set_sensor_type(PORT_1, SENSOR_TYPE_TOUCH);
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);
}

void IO::update() {
    BP.get_sensor(PORT_1, Touch1);
    touchSensor1 = !(Touch1.pressed);
    BP.get_sensor(PORT_2, Touch2);
    touchSensor2 = !(Touch2.pressed);
    BP.get_sensor(PORT_3, Light3);
    lightValue = Light3.reflected;
    BP.get_sensor(PORT_4, Ultrasonic4);
    distance = Ultrasonic4.cm;
}

void IO::calibrate() {
    while (true) {
        // wait for user input (druk sensor Nxt)
        if (!(BP.get_sensor(PORT_2, Touch2))) {
            update();
            white = lightValue;
            break;
        }
        usleep(10 * 1000);
    }
    
    while (true) {
        // wait for user input (druk sensor Nxt)
        if (!(BP.get_sensor(PORT_1, Touch1))) {
            update();
            black = lightValue;
            break;
        }
        usleep(10 * 1000);
    }
    average = white + ((black - white) / 2);
}

int IO::calcSpeed(){
    // foutwaarde is lightValue - average

    //if (lightValue > black){black = lightValue; average = (white + black)/2}
    //if (lightValue < white){white = lightValue; average = (white + black)/2} 


    float margin = lightValue - average;
    int percentageMarge;


    if (margin > 0) { // te zwart
        percentageMarge = (margin / (black - average) ) * 100.0;
        std::cout << "black: " << percentageMarge << std::endl;
    } else { //te wit
        percentageMarge = (margin / (average - white) ) * 100.0;
        std::cout << "white: " << percentageMarge << std::endl;
    }
    return percentageMarge;
}


void IO::resetEncoders() {
    BP.reset_motor_encoder(PORT_A);
    BP.reset_motor_encoder(PORT_B);
    BP.reset_motor_encoder(PORT_C);
}

void IO::dpsA(int speed) {
    // speed is between -100% and 100%;
    if (speed < -100) speed = -100;
    if (speed > 100) speed = 100;
    BP.set_motor_dps(PORT_A, (speed * MAX_SPEED)/100);
}

void IO::dpsB(int speed) { // linker motor
    // speed is between 0% and 100%;
    if (speed < -100) speed = -100;
    if (speed > 100) speed = 100;
    BP.set_motor_dps(PORT_B, (speed * MAX_SPEED)/100);
}

void IO::dpsC(int speed) { // rechter motor
    // speed is between 0% and 100%;
    if (speed < -100) speed = -100;
    if (speed > 100) speed = 100;
    BP.set_motor_dps(PORT_C, (speed * MAX_SPEED)/100);
}