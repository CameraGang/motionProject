#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include<unistd.h>

pthread_t ledThreadId;

void runCommandSing(char* command);


// void initCommands(void);


void startLed();


void setTrigger();


void stopLed(void);


int readFromFileToScreen(char *fileName);


// void sleepForMs(long long delayInMs);


void led_init();


void led_cleanup();


void *ledThread(void *arg);
	