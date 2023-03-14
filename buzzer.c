#include "buzzer.h"
#include "helper.h"

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


long long getTimeInMs(void)
{
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	long long seconds = spec.tv_sec;
	long long nanoSeconds = spec.tv_nsec;
	long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;
	return milliSeconds;
}

enum direction joystickDir(void){
	

	int U = readFromFileToScreen(DA_JOYSTICK_FILE_NAME_HERE_U);	
	if(U == 0){
		return UP;
	}

	int D = readFromFileToScreen(DA_JOYSTICK_FILE_NAME_HERE_D);	
	if(D == 0){
		return DOWN;
	}

	int L = readFromFileToScreen(DA_JOYSTICK_FILE_NAME_HERE_L);	
	if(L == 0){
		return LEFT;
	}

	int R = readFromFileToScreen(DA_JOYSTICK_FILE_NAME_HERE_R);	
	if(R == 0){
		return RIGHT;
	}
    int I = readFromFileToScreen(DA_JOYSTICK_FILE_NAME_HERE_I);	
	if(I == 0){
		return IN;
	}
	else{
		return NODIR;
	}
}


void runCommandSing(char* command){
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}


void initCommands(void){
    runCommandSing("sudo config-pin p9_22 pwm");


    runCommandSing("config-pin p8.14 gpio");
	runCommandSing("config-pin p8.15 gpio");
	runCommandSing("config-pin p8.16 gpio");
	runCommandSing("config-pin p8.18 gpio");
    runCommandSing("config-pin p8.17 gpio");

	 


	//setting the gpio to inport
	FILE *pFileA = fopen("/sys/class/gpio/gpio26/direction", "w");
	if (pFileA == NULL) {
		printf("ERROR: Unable to open export file A.\n");
		exit(1);
	}
	// Write to data to the file using fprintf():
	fprintf(pFileA, "%s", "in");
	// Close the file using fclose():
	fclose(pFileA);

	FILE *pFileB = fopen("/sys/class/gpio/gpio47/direction", "w");
	if (pFileB == NULL) {
		printf("ERROR: Unable to open export file B.\n");
		exit(1);
	}
	// Write to data to the file using fprintf():
	fprintf(pFileB, "%s", "in");
	// Close the file using fclose():
	fclose(pFileB);

	FILE *pFileC = fopen("/sys/class/gpio/gpio46/direction", "w");
	if (pFileC == NULL) {
		printf("ERROR: Unable to open export file C.\n");
		exit(1);
	}
	// Write to data to the file using fprintf():
	fprintf(pFileC, "%s", "in");
	// Close the file using fclose():
	fclose(pFileC);

	FILE *pFileD = fopen("/sys/class/gpio/gpio65/direction", "w");
	if (pFileD == NULL) {
		printf("ERROR: Unable to open export file D.\n");
		exit(1);
	}

	// Write to data to the file using fprintf():
	fprintf(pFileD, "%s", "in");
	// Close the file using fclose():
	fclose(pFileD);


    FILE *pFileE = fopen("/sys/class/gpio/gpio27/direction", "w");
	if (pFileE == NULL) {
		printf("ERROR: Unable to open export file E.\n");
		exit(1);	
	}

    // Write to data to the file using fprintf():
	fprintf(pFileE, "%s", "in");
	// Close the file using fclose():
	fclose(pFileE);
}



void startBuzzerA(int mode){
     

    FILE *pFileA = fopen("/dev/bone/pwm/0/a/duty_cycle", "w");
    if(pFileA == NULL){
        printf("ERROR: Unable to open buzzer duty_cycle");
    }
    FILE *pFileB = fopen("/dev/bone/pwm/0/a/period", "w");
    if(pFileB == NULL){
        printf("ERROR: Unable to open buzzer period");
    }
    FILE *pFileC = fopen("/dev/bone/pwm/0/a/enable", "w");
    if(pFileC == NULL){
        printf("ERROR: Unable to open buzzer enable");
    }

    // printf("%d\n", mode);

    if(mode == 0){
        fprintf(pFileA, "%d", 0);
    }


    else if(mode == 1 || mode == 2 || mode == 14){
        //buzzer noises        
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 3822256);
        fprintf(pFileA, "%d", 1991128);
        fprintf(pFileC, "%d", 1);
    }

    else if(mode == 3 || mode == 4 || mode == 7){
        //buzzer noises        
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 1275526);
        fprintf(pFileA, "%d", 637763);
        fprintf(pFileC, "%d", 1);

    }
    // //period cannot be higher than duty cycle
    else if(mode == 5 || mode == 6){
        //A5
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 1136364);
        fprintf(pFileA, "%d", 568182);
        fprintf(pFileC, "%d", 1);
    }

    else if(mode == 8 || mode == 9){
        //F4
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 2863442);
        fprintf(pFileA, "%d", 1431721);
        fprintf(pFileC, "%d", 1);
    }
    else if(mode == 10 || mode == 11){
        //E3
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 6067593);
        fprintf(pFileA, "%d", 3033796);
        fprintf(pFileC, "%d", 1);
    }
    else if(mode == 12 || mode == 13){
        //D2
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 13620267);
        fprintf(pFileA, "%d", 6810133);
        fprintf(pFileC, "%d", 1);
    }

    fclose(pFileA);
    fclose(pFileB);
    fclose(pFileC);

}

void startBuzzerB(int mode){
     

    FILE *pFileA = fopen("/dev/bone/pwm/0/a/duty_cycle", "w");
    if(pFileA == NULL){
        printf("ERROR: Unable to open buzzer duty_cycle");
    }
    FILE *pFileB = fopen("/dev/bone/pwm/0/a/period", "w");
    if(pFileB == NULL){
        printf("ERROR: Unable to open buzzer period");
    }
    FILE *pFileC = fopen("/dev/bone/pwm/0/a/enable", "w");
    if(pFileC == NULL){
        printf("ERROR: Unable to open buzzer enable");
    }

    // printf("%d\n", mode);

    if(mode == 0){
        fprintf(pFileA, "%d", 0);
    }


    else if(mode == 1 || mode == 2 || mode == 4 || mode == 7 || mode == 8 || mode == 10 || mode == 13 || mode == 14){
        //G4
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 2551020);
        fprintf(pFileA, "%d", 1275510);
        fprintf(pFileC, "%d", 1);       
        
    }

    else if(mode == 3 || mode == 9 || mode == 19){
        //A3   
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 4545455);
        fprintf(pFileA, "%d", 2272727);
        fprintf(pFileC, "%d", 1); 
        
    }
    // //period cannot be higher than duty cycle
    else if(mode == 5 || mode == 12 || mode == 17 || mode == 23 || mode == 25){
        //C1
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 3822256);
        fprintf(pFileA, "%d", 1991128);
        fprintf(pFileC, "%d", 1);
    }

    else if(mode == 6 || mode == 18){
        //B2
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 8099134);
        fprintf(pFileA, "%d", 4049567);
        fprintf(pFileC, "%d", 1); 
        
    }
    else if(mode == 11 || mode == 24){
        //D2
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 13620267);
        fprintf(pFileA, "%d", 6810133);
        fprintf(pFileC, "%d", 1);
        
    }
    else if(mode == 15){
        //G5
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 1275526);
        fprintf(pFileA, "%d", 637763);
        fprintf(pFileC, "%d", 1);
    }
    else if(mode == 16 || mode == 22){
        //E3
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 6067593);
        fprintf(pFileA, "%d", 3033796);
        fprintf(pFileC, "%d", 1);
    }
    else if(mode == 19 || mode == 20){
        //F4
        fprintf(pFileA, "%d", 0);
        fprintf(pFileB, "%d", 2863442);
        fprintf(pFileA, "%d", 1431721);
        fprintf(pFileC, "%d", 1);
    }

    fclose(pFileA);
    fclose(pFileB);
    fclose(pFileC);

}


void stopBuzzer(void){

    FILE *pFileC = fopen("/dev/bone/pwm/0/a/enable", "w");
    if(pFileC == NULL){
        printf("ERROR: Unable to open buzzer enable");
    }

    //turn on buzzer
    fprintf(pFileC, "%d", 0);
    fclose(pFileC);
}

int modeSetter(int currMode){
    if(currMode != 3){
        currMode++;
    }
    else{
        currMode = 0;
    }
    return currMode;
}

int readFromFileToScreen(char *fileName)
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

void Buzzer_init()
{
    initCommands();
    pthread_create(&buzzerThreadId, NULL, &buzzerThread, NULL);
}

void Buzzer_cleanup()
{
    stopBuzzer();
    pthread_join(buzzerThreadId, NULL);
}

void *buzzerThread(void *arg){
// int main(){

    //remove later
    initCommands();

    int WLT = 1;

    
    int modeA = 0;
    int modeB = 0;

    long long prevTime = 0;

    int sleepOff = 20;
    int sleepOn = 450;

    int songA = 1; 
    int songB = 0; 
    int songMute = 0;

    while(WLT){

       
        enum direction dirIn = NODIR;
            
        while(songA){

            long long currTime = getTimeInMs();
            dirIn = joystickDir();
            if(dirIn == RIGHT){
                if(currTime - prevTime >= 500){
                    songA = 0;
                    songB = 1;
                    modeB = 0;
                    stopBuzzer(); 
                    sleepForMs(1000); 
                    prevTime = currTime;
                }
                
            }
            else if(dirIn == IN){
                if(currTime - prevTime >= 500){
                    songA = 0;
                    songMute = 1;
                    prevTime = currTime;
                }
            }
            else if(dirIn == LEFT){
                if(currTime - prevTime >= 500){
                    songA = 0;
                    WLT = 0;
                }        
            }
            else if(dirIn == UP){
                if(currTime - prevTime >= 500){
                    sleepOff -= 5;
                    sleepOn -= 5;
                    prevTime = currTime;
                }
            }
            else if(dirIn == DOWN){
                if(currTime - prevTime >= 500){
                    sleepOff += 5;
                    sleepOn += 5;
                    prevTime = currTime;
                }
            }

            if(modeA != 14){
                modeA++;
            }
            else{
                modeA = 0;
            }
            sleepForMs(sleepOff);
            startBuzzerA(modeA);
            sleepForMs(sleepOn);
            stopBuzzer();
        }

        while(songB){
            long long currTime = getTimeInMs();
            dirIn = joystickDir();
            if(dirIn == RIGHT){
                if(currTime - prevTime >= 500)
                songA = 1;
                songB = 0; 
                modeA = 0;
                stopBuzzer();
                sleepForMs(1000);
                prevTime = currTime; 
            }
               
            else if(dirIn == IN){
                if(currTime - prevTime >= 500){
                    songB = 0;
                    songMute = 1;
                    prevTime = currTime;
                }
            }
            else if(dirIn == LEFT){
                if(currTime - prevTime >= 500){
                    songB = 0;
                    WLT = 0;
                }        
            }
            else if(dirIn == UP){
                if(currTime - prevTime >= 500){
                    printf("%d\n",sleepOff);
                    printf("%d\n",sleepOn);
                    sleepOff -= 5;
                    sleepOn -= 5;
                    prevTime = currTime;
                }
            }
            else if(dirIn == DOWN){
                if(currTime - prevTime >= 500){
                    printf("%d\n",sleepOff);
                    printf("%d\n",sleepOn);
                    sleepOff += 5;
                    sleepOn += 5;
                    prevTime = currTime;
                }
            }

            if(modeB != 25){
                modeB++;
            }
            else{
                modeB = 0;
            }
            sleepForMs(sleepOff);
            startBuzzerB(modeB);
            sleepForMs(sleepOn);
            stopBuzzer();
            
        }
        while(songMute){
            stopBuzzer();
            long long currTime = getTimeInMs();
            dirIn = joystickDir();
            if(dirIn == IN){
                if(currTime - prevTime >= 500){
                    songMute = 0;
                    songA = 1;
                    songB = 0;
                    modeA = 0;
                    prevTime = currTime;
                }
            }
        }
    }
    stopBuzzer();
    return 0;
}