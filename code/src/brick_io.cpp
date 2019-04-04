
#include "../include/brick_io.hpp"
#include <unistd.h>  // for usleep
#include <iostream>
#include "../include/helpmath.hpp"

IO::IO() {
    BP.detect();

    BP.set_sensor_type(PORT_1, SENSOR_TYPE_TOUCH);
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_TOUCH);
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);

    resetEncoders();
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

int IO::calcSpeed() {
    int maxspeed = 100;
    int speed = (int)map<float>(lightValue, white, black, -maxspeed, maxspeed);

    if (speed < -maxspeed)
        speed = -maxspeed;
    else if (speed > maxspeed)
        speed = maxspeed;
    return speed;
}

void IO::resetEncoders() {
    BP.reset_motor_encoder(PORT_A);
    BP.reset_motor_encoder(PORT_B);
    BP.reset_motor_encoder(PORT_C);
}

void IO::dpsA(int speed) {  // extra motor
    // speed is between -100% and 100%;
    if (speed > 100) speed = 100;
    speedA = speed;
    BP.set_motor_dps(PORT_A, (speed * MAX_SPEED) / 100);
}

void IO::dpsB(int speed) {  // linker motor
    // speed is between 0% and 100%;
    if (speed > 100) speed = 100;
    speedB = speed;
    BP.set_motor_dps(PORT_B, ((speed * MAX_SPEED) / 100));
}

void IO::dpsC(int speed) {  // rechter motor
    // speed is between 0% and 100%;
    if (speed > 100) speed = 100;
    speedC = speed;
    BP.set_motor_dps(PORT_C, ((speed * MAX_SPEED) / 100));
}

void IO::steerPosition(int pos) {
    if (pos > 100) pos = 100;
    if (pos < -100) pos = -100;

    BP.set_motor_position(PORT_A, pos);
}