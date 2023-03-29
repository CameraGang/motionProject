#include "led.h"
#include "helper.h"
#include "sharedFunc.h"

pthread_t ledThreadId;

static int ledOn = 0;
static int running = 1;

//LIKELY NEEDS CHANGING
static void initCommandsLed(void){

    // runCommandSing("sudo config-pin p9.23 gpio");

	//setting the gpio to inport
	FILE *pFileA = fopen("/sys/class/gpio/gpio49/direction", "w");
	if (pFileA == NULL) {
		printf("ERROR: Unable to open export file A.\n");
		exit(1);
	}
	// Write to data to the file using fprintf():
	fprintf(pFileA, "%s", "out");
	// Close the file using fclose():
	fclose(pFileA);
}


static void setTrigger(){
	FILE *pFileA = fopen("/sys/class/leds/leds:P9.23/trigger", "w");
    if(pFileA == NULL){
        printf("ERROR: Unable to open LED delay file");
    }	
	fprintf(pFileA, "%s", "timer");
	fclose(pFileA);
}


static void startLed(){

	FILE *pFileA = fopen("/sys/class/leds/leds:P9.23/brightness", "w");
	
	if(pFileA == NULL){
			printf("ERROR: Unable to LED file");
	}

	fprintf(pFileA, "%d", 1);
	fclose(pFileA);

	FILE *pFileB = fopen("/sys/class/leds/leds:P9.23/delay_on", "w");
	if(pFileB == NULL){
		printf("ERROR: Unable to open LED trigger file delay_on\n");
		return;
	}

	FILE *pFileC = fopen("/sys/class/leds/leds:P9.23/delay_off", "w");
	if(pFileC == NULL){
			printf("ERROR: Unable to open LED trigger file delay_off");
	}

	fprintf(pFileB, "%d", 100);
	fprintf(pFileC, "%d", 100);
	fclose(pFileB);
	fclose(pFileC);
}


static void stopLed(){
	FILE *pFileA = fopen("/sys/class/leds/leds:P9.23/brightness", "w");
	
	if(pFileA == NULL){
			printf("ERROR: Unable to LED file");
	}

	fprintf(pFileA, "%d", 0);
	fclose(pFileA);
}

void led_init(){
	initCommandsLed(); 
	setTrigger();
  pthread_create(&ledThreadId, NULL, &ledThread, NULL);
}


void led_cleanup(){
	running = 0;
  pthread_join(ledThreadId, NULL);
}

void *ledThread(void *arg){
	while (running == 1) {
		if (ledOn == 1) {
			startLed();
		} else {
			stopLed();
		}

		sleepForMs(100);
	}

	pthread_exit(NULL);
}

void led_start() {
	ledOn = 1;
}

void led_stop() {
	ledOn = 0;
}
