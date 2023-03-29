#ifndef BUZZER_H
#define BUZZER_H

//Module for handling the buzzer functionality and setup
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>



#define DA_JOYSTICK_FILE_NAME_HERE_U "/sys/class/gpio/gpio26/value"
#define DA_JOYSTICK_FILE_NAME_HERE_D "/sys/class/gpio/gpio46/value"
#define DA_JOYSTICK_FILE_NAME_HERE_L "/sys/class/gpio/gpio65/value"
#define DA_JOYSTICK_FILE_NAME_HERE_R "/sys/class/gpio/gpio47/value"
#define DA_JOYSTICK_FILE_NAME_HERE_I "/sys/class/gpio/gpio27/value"

enum direction {
		UP, DOWN, LEFT, RIGHT, NODIR, IN
};

int readFromFileToScreenBuzz(char *fileName);

void Buzzer_init();

void Buzzer_cleanup();

void *buzzerThread(void *arg);
	
void Buzzer_enableBuzz();
void Buzzer_disableBuzz(); 
#endif	