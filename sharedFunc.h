//contains functions that are shared by led and buzzer module

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

long long getTimeInMs(void);


void runCommandSing(char* command);