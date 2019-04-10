/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering
*/

#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

void omx(const int &soundIndex) {
    int pid = fork();
    if (pid == 0) {
        char *sound[] = {
            "/home/pi/BrickPiProject/code/sounds/thisdood.mp3",
            "/home/pi/BrickPiProject/code/sounds/dejavu.mp3",
            "/home/pi/BrickPiProject/code/sounds/gas.mp3",
            "/home/pi/BrickPiProject/code/sounds/dawey.mp3",
            "/home/pi/BrickPiProject/code/sounds/treintoeter.mp3",
            "/home/pi/BrickPiProject/code/sounds/soviet-anthem.mp3"};

        char *arguments[] = {"/usr/bin/omxplayer", "--vol", "-600",
                             sound[soundIndex], NULL};
        execv("/usr/bin/omxplayer", arguments);
    }
}

int main() {
    while (true) {
        omx(1);

        int exit_status;
        wait(&exit_status);
    }

    usleep(100 * 1000);
    return 0;
}