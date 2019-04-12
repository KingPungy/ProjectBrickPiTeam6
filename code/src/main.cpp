/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering

*/
#include <signal.h>  // for catching exit signals
#include <termios.h>
#include <unistd.h>  // for usleep
#include <cmath>
#include <cstdio>  // for printf
#include <cstdlib>

// Class headers
#include "../include/brick_io.hpp"    // IO input Class
#include "../include/controller.hpp"  // Controller input Class
                                      // hulp functies

#include "../include/helpmath.hpp"
#include "../include/server.h"

#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

#define DEBUG

#ifdef DEBUG
#define DEBUGP(a...) printf(a)
#endif

void exit_signal_handler(int signo);
// classControl controller;
IO dotIO;
classControl controller;
const std::vector<std::string> sounds = {
    "/home/pi/BrickPiProject/code/sounds/thisdood.mp3",
    "/home/pi/BrickPiProject/code/sounds/dejavu.mp3",
    "/home/pi/BrickPiProject/code/sounds/gas.mp3",
    "/home/pi/BrickPiProject/code/sounds/dawey.mp3",
    "/home/pi/BrickPiProject/code/sounds/treintoeter.mp3",
    "/home/pi/BrickPiProject/code/sounds/soviet-anthem.mp3"};

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

int omx(const int &soundIndex) {
    if (soundIndex < 0 || soundIndex >= sounds.size()) return -1;
    int pid = fork();
    if (pid == 0) {
        // FIXME
        return execl("/usr/bin/omxplayer", "/usr/bin/omxplayer", "--vol",
                     "-500", sounds[soundIndex].c_str(), (char *)0);
    }
    return -pid;
}

float lerp(float a, float b, float f) { return (a * (1.0 - f)) + (b * f); }

int main(int argc, char *argv[]) {
    signal(SIGINT, exit_signal_handler);  // register exit for Ctrl+C
    dotIO.dpsB(0);
    dotIO.dpsC(0);

    int args = 1;
    /*
    Different Flags:
        Controller : Use the Xbox Controller with triggers and left JoyStick.
        Speed      : Set maximum speed value to increase or reduce control over
    the robot.
    */
    bool controllerFlag = false;
    uint8_t check_speed = 0;

    // system("sudo xboxdrv --detach-kernel-driver --silent &");

    if (argc > 1)
        if (argv[1][0] == '-')
            for (int i = 1; i < strlen(argv[1]); i++) switch (argv[1][i]) {
                    case 's':
                        check_speed = ++args;
                        break;
                    case 'c':
                        controllerFlag = true;
                        break;
                }

    if (check_speed && check_speed < argc)
        dotIO.MAX_SPEED = atoi(argv[check_speed]);

    printf("controller mode: %s\n", controllerFlag ? "on" : "off");
    printf("max speed: %d\n", dotIO.MAX_SPEED);

    // float steerto = 0.0;
    // float steerto_old = 0.0;
    // float percentage = 0.0;

    server serv(DEFAULT_PORT);

    if (controllerFlag) {
        int value = -1;
        int old_value = value;

        while (true) {
            dotIO.update();
            // std::cout << "\r" << dotIO.speedA << "\t" << dotIO.speedB << "\t"
            //          << dotIO.speedC << "\t" << dotIO.redValue << "\t"
            //          << dotIO.greenValue << "\t" << dotIO.blueValue
            //          << std::endl;

            if ((dotIO.redValue > 420 and dotIO.redValue < 460) and
                (dotIO.greenValue > 400 and dotIO.greenValue < 440) and
                (dotIO.blueValue > 230 and dotIO.blueValue < 270)) {
                std::cout << "Insitutieplein!" << std::endl;
                // if sensor detects dark grey
            } else if ((dotIO.redValue > 380 and dotIO.redValue < 420) and
                       (dotIO.greenValue > 370 and dotIO.greenValue < 410) and
                       (dotIO.blueValue > 280 and dotIO.blueValue < 320)) {
                std::cout << "Donkergrijs" << std::endl;
                // if sensor detects light grey
            } else if ((dotIO.redValue > 430 and dotIO.redValue < 470) and
                       (dotIO.greenValue > 420 and dotIO.greenValue < 460) and
                       (dotIO.blueValue > 330 and dotIO.blueValue < 370)) {
                std::cout << "Lichtgrijs" << std::endl;
                // if sensor detects orange
            } else if ((dotIO.redValue > 570 and dotIO.redValue < 650) and
                       (dotIO.greenValue > 320 and dotIO.greenValue < 400) and
                       (dotIO.blueValue > 170 and dotIO.blueValue < 250)) {
                std::cout << "Oranje stop nu!!!" << std::endl;
                dotIO.dpsB(60);
                dotIO.dpsC(60);
                usleep(1000 * 1000);
                dotIO.dpsB(0);
                dotIO.dpsC(0);
            } else if (dotIO.lightValue > 2400 && dotIO.lightValue < 2800) {
                // if sensor detects the shade of the stairs
                std::cout << "trap!!" << std::endl;
                dotIO.dpsB(60);
                dotIO.dpsC(60);
                usleep(1000 * 1000);
                dotIO.dpsB(0);
                dotIO.dpsC(0);
            }
            if (false && value != old_value) {
                switch (value) {
                    case -1:  // printf("Niet bekend.\n");
                        break;
                    case 0:
                        printf("Insitutieplein!\n");
                        break;
                    case 1:
                        printf("Donkergrijs\n");
                        break;
                    case 2:
                        printf("Lichtgrijs\n");
                        break;
                }
            }

            old_value = value;

            if (serv.has_message()) {
                // printf("got message: ");
                message msg = serv.get_message();
                // printf("id = %d, size = %d\n", msg.s.id, msg.s.size);

                switch (msg.s.id) {
                    case MESSAGE_ID_INPUT_CONTROLLER_BTN_CHANGE:
                        controller.process_input_controller_btn_change(
                            (void *)msg.data);
                        break;
                    case MESSAGE_ID_INPUT_CONTROLLER_BTN_ALL:
                        controller.process_input_controller_btn_all(
                            (void *)msg.data);
                        break;
                    default:
                        break;
                }
            }

            int exit_status;
            int ret =
                waitpid(0, &exit_status, WNOHANG | WUNTRACED | WCONTINUED);
            // FIXME
            if (ret == -1 || ret == 0) {
                if (controller.a()) {
                    omx(4);  // treintoeter
                } else if (controller.b()) {
                    omx(3);  // da wae
                } else if (controller.x()) {
                    omx(1);  // dejavu
                } else if (controller.y()) {
                    omx(0);  // this dood
                }
            }

            // controller.printInput();

            // // so the joystick is non-linear

            float joyStick = controller.lJoyX();  // sqrt(fabs(lJoyX)) * lJoyX <
                                                  // 0 ? -10.0 : 10.0;
            float steering = joyStick;
            /*
            if (fabs(joyStick - steerto) > 0.5)
            {
                //percentage *= (joyStick + 100.0) / (steerto + 100.0);
                //steerto_old = steerto;
                percentage = 0.0;
                steerto = joyStick;
            }

            float steering = percentage * steerto;// lerp(steerto, steerto,
            percentage); //map<float>(percentage, 0.0, 1.0, 0.0, steerto);
            */
            float speed = controller.lTrig() - controller.rTrig();

            // printf("joyStick = %.4f, steering = %.4f, percentage =
            // %.4f\n", joyStick, steering, percentage);

            // this is to slow down the rotation of the wheel that the car
            // is turning to
            float lJoyXl = 1.0;
            float lJoyXr = 1.0;
            if (steering > 0)
                lJoyXl -= steering / 1000.0;
            else if (steering < 0)
                lJoyXr -= steering / 1000.0;

            float left = speed * lJoyXl;
            float right = speed * lJoyXr;

            // Uses the mapped values of the triggers to determine the speed
            // of the robot
            dotIO.setLeft((int)left);
            dotIO.setRight((int)right);

            // Sets the position of the motor for the front steeing wheel ,
            // Motor A

            dotIO.steerPosition((int)steering);
            /*
            percentage += 0.01;
            if (percentage > 1.0)
                percentage = 1.0;
            */
        }
    } else {
        while (true) {
            dotIO.update();
            char buf = 0;
            switch (buf = getch(0)) {
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
            dotIO.steerPosition(turn * 2);
        }
    }
}

// Signal handler that will be called when Ctrl+C is pressed to stop the
// program
void exit_signal_handler(int signo) {
    // printf("signal: %d\n", signo);
    // Reset everything so there are no run-away motors
    dotIO.resetMotors();
    exit(signo);
}
