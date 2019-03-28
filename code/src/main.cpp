/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering

*/
#include <signal.h>  // for catching exit signals
#include <stdio.h>   // for printf
#include <unistd.h>  // for usleep
#include <termios.h>
//#include "BrickPi3.h"  // for BrickPi3

// Class headers
#include "../include/brick_io.hpp"    // IO input Class
#include "../include/controller.hpp"  // Controller input Class

void exit_signal_handler(int signo);
// classControl controller;
IO dotIO;

char getch(int vmin = 1, int vtime = 0)
{
    char buf = 0;
    struct termios temp = { 0 };

    tcgetattr(STDOUT_FILENO, &temp);

    temp.c_lflag &= ~ICANON;
    temp.c_cc[VMIN] = vmin;
    temp.c_cc[VTIME] = vtime;

    tcsetattr(STDOUT_FILENO, TCSANOW, &temp);

    read(STDOUT_FILENO, &buf, 1);

    tcsetattr(STDOUT_FILENO, TCSADRAIN, &temp);
    
    return buf;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, exit_signal_handler);  // register exit for Ctrl+C

    bool manual = false;

    if (argc > 1)
        if (argv[1][0] == '-')
            for (int i = 1; i < strlen(argv[1]); i++)
                switch (argv[1][i])
                {
                    case 'm': manual = true; break;
                }

    if (manual)
    {
        int speed = 50;
        int maxspeed = 100;

        bool forward = 0;
        bool backward = 0;
        bool turn_left = 0;
        bool turn_right = 0;

        int left = 0;
        int right = 0;

        while (true)
        {
            char buf = 0;
            switch (buf = getch(0))
            {
                case 'w': case 'W':
                    if (backward)
                    {
                        forward = 0;
                        backward = 0;
                    }
                    else
                    {
                        forward = 1;
                        backward = 0;
                    }
                break;
                case 's': case 'S':
                    if (forward)
                    {
                        forward = 0;
                        backward = 0;
                    }
                    else
                    {
                        forward = 0;
                        backward = 1;
                    }
                break;
                case 'a': case 'A':
                    if (turn_right)
                    {
                        turn_left = 0;
                        turn_right = 0;
                    }
                    else
                    {
                        turn_left = 1;
                        turn_right = 0;
                    }
                break;
                case 'd': case 'D':
                    if (turn_left)
                    {
                        turn_left = 0;
                        turn_right = 0;
                    }
                    else
                    {
                        turn_left = 0;
                        turn_right = 1;
                    }
                break;
            }

            if (turn_left)
            {
                if (forward)
                {
                    left = speed;
                    right = maxspeed;
                }
                else if (backward)
                {
                    left = -speed;
                    right = -maxspeed;
                }
                else
                {
                    left = 0;
                    right = maxspeed;
                }
            }
            else if (turn_right)
            {
                if (forward)
                {
                    left = maxspeed;
                    right = speed;
                }
                else if (backward)
                {
                    left = -maxspeed;
                    right = -speed;
                }
                else
                {
                    left = maxspeed;
                    right = 0;
                }
            }
            else
            {
                if (forward)
                {
                    left = maxspeed;
                    right = maxspeed;
                }
                else if (backward)
                {
                    left = -maxspeed;
                    right = -maxspeed;
                }
                else
                {
                    left = 0;
                    right = 0;
                }
            }
            

            if (left > maxspeed)
                left = maxspeed;
            else if (left < -maxspeed)
                left = -maxspeed;
            
            if (right > maxspeed)
                right = maxspeed;
            else if (right < -maxspeed)
                right = -maxspeed;

            dotIO.dpsB(left);
            dotIO.dpsC(right);
        }
    }
    else
    {
        while (true) {
            usleep(1 * 1000);
            dotIO.update();

            int speed2 = dotIO.calcSpeed();

            int speed = speed2 / 2;


            printf("\rdistance: %8d, brightness: %8d, speed: %16d", dotIO.distance, dotIO.lightValue, speed2);
            if (!dotIO.touchSensor1) {
                // printf("touch sensor 1: %d\n", dotIO.lightValue);
                dotIO.black = dotIO.lightValue;
            }
            if (!dotIO.touchSensor2) {
                // printf("touch sensor 2: %d\n", dotIO.lightValue);
                dotIO.white = dotIO.lightValue;
            }
            if (!dotIO.touchSensor1 || !dotIO.touchSensor2) {
                dotIO.dpsB(0);
                dotIO.dpsC(0);
                dotIO.average = (dotIO.black + dotIO.white) / 2.0;
                continue;
            }

            if (dotIO.distance < 5) {
                
                dotIO.dpsB(0);
                dotIO.dpsC(0);

                continue;
            }

            int maxspeed = 50;
            int margin = 5;

            if (speed < margin) {
                speed -= maxspeed;
                dotIO.dpsB(maxspeed * 2 - abs(speed));
                dotIO.dpsC(abs(speed));
            } else if (speed > margin) {
                speed += maxspeed;
                dotIO.dpsB(speed);
                dotIO.dpsC(maxspeed * 2 - speed);
            } else {
                dotIO.dpsB(maxspeed);
                dotIO.dpsC(maxspeed);
            }
        }
    }
    dotIO.dpsB(0);
    dotIO.dpsC(0);
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo) {
    printf("signal: %d\n", signo);
    dotIO.dpsA(0);
    dotIO.dpsB(0);
    dotIO.dpsC(0);  // Reset everything so there are no run-away motors
    exit(-2);
}