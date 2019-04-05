/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering

*/
#include <cmath>
#include <cstdio>   // for printf
#include <cstdlib>
#include <signal.h>  // for catching exit signals
#include <unistd.h>  // for usleep
#include <termios.h>

// Class headers
#include "../include/brick_io.hpp"    // IO input Class
#include "../include/controller.hpp"  // Controller input Class
                                      // hulp functies

#include "../include/server.h"

#define DEBUG

#ifdef DEBUG
    #define DEBUGP(a...) printf(a)
#endif

void exit_signal_handler(int signo);
// classControl controller;
IO dotIO;
classControl controller;

char getch(int vmin = 1, int vtime = 0) {
    char buf = 0;
    struct termios temp = {0};

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

    int args = 1;
    /*
    Different Flags:
        Manual     : Use W,A,S,D to control the robot.
        Controller : Use the Xbox Controller with triggers and left JoyStick.
        Speed      : Set maximum speed value to increase or reduce control over the robot.
    */
    bool controllerFlag = false;
    uint8_t check_speed = 0;

    //system("sudo xboxdrv --detach-kernel-driver --silent &");

    if (argc > 1)
        if (argv[1][0] == '-')
            for (int i = 1; i < strlen(argv[1]); i++)
                switch (argv[1][i])
                {
                    case 's':
                        check_speed = ++args;
                        break;
                    case 'c':
                        controllerFlag = true;
                        break;
                }
    
    if (check_speed && check_speed < argc)
        dotIO.MAX_SPEED = atoi(argv[check_speed]);

    printf("max speed: %d\n", dotIO.MAX_SPEED);
    printf("controller mode: %s\n", controllerFlag ? "on" : "off");

    server serv(DEFAULT_PORT);

    if (controllerFlag)
    {
        while (true)
        {
            if (serv.has_message())
            {
                message msg = serv.get_message();
                printf("got message. id: %d, size: %d\n", msg.s.id, msg.s.size);

                switch (msg.s.id)
                {
                    case MESSAGE_ID_INPUT_CONTROLLER_BTN_CHANGE:
                        controller.process_input((input_event*)msg.data);
                        
                        if (controller.start)
                        {
                            controller.lTrig = 0;
                            controller.rTrig = 0;
                            
                            controller.lJoyX = 0;
                            controller.rJoyX = 0;
                            controller.lJoyY = 0;
                            controller.rJoyY = 0;
                        }
                        if (controller.dUpDown != 0)
                        {
                            dotIO.MAX_SPEED += controller.dUpDown * 100;
                            if (dotIO.MAX_SPEED > 1000)
                                dotIO.MAX_SPEED = 1000;
                            else if (dotIO.MAX_SPEED < 100)
                                dotIO.MAX_SPEED = 100;
                        }
                        break;
                    case MESSAGE_ID_INPUT_CONTROLLER_BTN_ALL:
                        break;
                    default:
                        break;
                }
            }

            float lTrig = sqrt(controller.lTrig) * 10.0;
            float rTrig = sqrt(controller.rTrig) * 10.0;
            
            float wspeed = lTrig - rTrig;
            float lJoyXl = 1.0;
            if (controller.lJoyX > 0)
                lJoyXl -= controller.lJoyX / 1000.0;
            float lJoyXr = 1.0;
            if (controller.lJoyX < 0)
                lJoyXr -= controller.lJoyX / -1000.0;
            float left = wspeed * lJoyXl;
            float right = wspeed * lJoyXr;

            // Uses the mapped values of the triggers to determine the speed of the robot
            dotIO.setLeft((int)left);
            dotIO.setRight((int)right);

            // Sets the position of the motor for the front steeing wheel , Motor A
            dotIO.steerPosition(controller.lJoyX);
        }
    } else {
        int speed = 50;

        int forward = 0;
        int turn = 0;

        while (true) {
            char buf = 0;
            switch (buf = getch(0))
            {
                case 's':
                case 'S':
                    if (forward > 0)
                        forward = 0;
                    else
                        forward = -speed;
                    break;
                case 'w':
                case 'W':
                    if (forward < 0)
                        forward = 0;
                    else
                        forward = speed;
                    break;
                case 'd':
                case 'D':
                    if (turn < 0)
                        turn = 0;
                    else
                        turn = speed;
                    break;
                case 'a':
                case 'A':
                    if (turn > 0)
                        turn = 0;
                    else
                        turn = -speed;
                    break;
            }

            dotIO.setLeft(-forward);
            dotIO.setRight(-forward);
            dotIO.steerPosition(turn);
        }
    }
    exit_signal_handler(0);
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo) {
    //printf("signal: %d\n", signo);
    // Reset everything so there are no run-away motors
    dotIO.steerPosition(0);
    dotIO.setLeft(0);
    dotIO.setRight(0);
    exit(-signo);
}
