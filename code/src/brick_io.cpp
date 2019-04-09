
#include "../include/brick_io.hpp"
#include <unistd.h>  // for usleep
#include <iostream>
#include <string>
#include "../include/helpmath.hpp"

IO::IO() { // Initialize sensors
    BP.detect();

    // BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
    // BP.set_sensor_type(PORT_1, SENSOR_TYPE_TOUCH);
    // BP.set_sensor_type(PORT_2, SENSOR_TYPE_TOUCH);
    // BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
}

void IO::update() {
    BP.get_sensor(PORT_1, Color1);
    redValue = Color1.reflected_red;
    greenValue = Color1.reflected_green;
    blueValue = Color1.reflected_blue;

    // transfer to file
    logClass.write(redValue, greenValue, blueValue, speedA, speedB, speedC);
}

int IO::calcSpeed() { // calculates speed based on black and white values
    int maxspeed = 100;
    int speed = (int)map<float>(lightValue, white, black, -maxspeed, maxspeed);

    if (speed < -maxspeed)
        speed = -maxspeed;
    else if (speed > maxspeed)
        speed = maxspeed;
    return speed;
}

void IO::resetEncoders() { // set 0 positions to all motors
    BP.reset_motor_encoder(PORT_B);
    BP.reset_motor_encoder(PORT_C);
    BP.reset_motor_encoder(PORT_D);
}

void IO::dpsD(int speed) {  // Front Steering motor
    // speed is between -100% and 100%;
    if (speed < -100) speed = -100;
    if (speed > 100) speed = 100;
    speedA = speed;
    BP.set_motor_dps(PORT_D, (speed * MAX_SPEED) / 100);
}

void IO::dpsB(int speed) {  // motor
    // speed is between -100% and 100%;
    if (speed < -100) speed = -100;
    if (speed > 100) speed = 100;
    speedB = speed;
    BP.set_motor_dps(PORT_B, ((speed * MAX_SPEED) / 100));
}

void IO::dpsC(int speed) {  // motor
    // speed is between -100% and 100%;
    if (speed < -100) speed = -100;
    if (speed > 100) speed = 100;
    speedC = speed;
    BP.set_motor_dps(PORT_C, ((speed * MAX_SPEED) / 100));
}

void IO::setLeft(int speed)
{
    dpsB(speed);
}

void IO::setRight(int speed)
{
    dpsC(speed);
}

void IO::steerPosition(int pos) { // Maps the incoming values to the maximum and minimum steering positions
    if (pos > 100) pos = 100;
    if (pos < -100) pos = -100;

    int spos = (int)map<float>(pos, -100, 100, -maxSteering, maxSteering);

    BP.set_motor_position(PORT_A, spos);
}