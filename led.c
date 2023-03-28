#include "led.h"
#include "helper.h"
#include "sharedFunc.h"

pthread_t ledThreadId;

// long long getTimeInMs(void)
// {
// 	struct timespec spec;
// 	clock_gettime(CLOCK_REALTIME, &spec);
// 	long long seconds = spec.tv_sec;
// 	long long nanoSeconds = spec.tv_nsec;
// 	long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;
// 	return milliSeconds;
// }


// void sleepForMs(long long delayInMs)
// {
// 	const long long NS_PER_MS = 1000 * 1000;
// 	const long long NS_PER_SECOND = 1000000000;
// 	long long delayNs = delayInMs * NS_PER_MS;
// 	int seconds = delayNs / NS_PER_SECOND;
// 	int nanoseconds = delayNs % NS_PER_SECOND;
// 	struct timespec reqDelay = {seconds, nanoseconds};
// 	nanosleep(&reqDelay, (struct timespec *) NULL);
// }

// void runCommandSing(char* command){
//     // Execute the shell command (output into pipe)
//     FILE *pipe = popen(command, "r");
//     // Ignore output of the command; but consume it
//     // so we don't get an error when closing the pipe.
//     char buffer[1024];
//     while (!feof(pipe) && !ferror(pipe)) {
//         if (fgets(buffer, sizeof(buffer), pipe) == NULL)
//             break;
//         // printf("--> %s", buffer); // Uncomment for debugging
//     }
//     // Get the exit code from the pipe; non-zero is an error:
//     int exitCode = WEXITSTATUS(pclose(pipe));
//     if (exitCode != 0) {
//         perror("Unable to execute command:");
//         printf(" command: %s\n", command);
//         printf(" exit code: %d\n", exitCode);
//     }
// }


//LIKELY NEEDS CHANGING
void initCommandsLed(void){

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


void setTrigger(){
	FILE *pFileA = fopen("/sys/class/leds/leds:P9.23/trigger", "w");
    if(pFileA == NULL){
        printf("ERROR: Unable to open LED delay file");
    }	
	fprintf(pFileA, "%s", "timer");
	fclose(pFileA);
}


void startLed(){

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


void stopLed(){
	FILE *pFileA = fopen("/sys/class/leds/leds:P9.23/brightness", "w");
    if(pFileA == NULL){
        printf("ERROR: Unable to LED file");
    }
	fprintf(pFileA, "%d", 0);
	fclose(pFileA);
}

int readFromFileToScreenLed(char *fileName)
{
	FILE *pFile = fopen(fileName, "r");
	if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
	}
	// Read string (line)
	const int MAX_LENGTH = 1024;
	char buff[MAX_LENGTH];
	fgets(buff, MAX_LENGTH, pFile);
	// Close
	fclose(pFile);
	return(atoi(buff));
}

void led_init(){
	initCommandsLed(); 
    pthread_create(&ledThreadId, NULL, &ledThread, NULL);
}


void led_cleanup(){
    stopLed();
    pthread_join(ledThreadId, NULL);
}

void *ledThread(void *arg){
// int main(){
	setTrigger();
	sleepForMs(100);
	startLed();
	return 0;
}

