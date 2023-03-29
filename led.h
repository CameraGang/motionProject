#ifndef LED_H
#define LED_H

#include <stdio.h>

#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include<unistd.h>

void led_init();

void led_cleanup();

void *ledThread(void *arg);

void led_start();

void led_stop();

#endif	