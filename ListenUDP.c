#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include "ListenUDP.h"
#include "helper.h"
#include "camera.h"
#include "buzzer.h"

static struct sockaddr_in serverAddr, cliAddr;
char udpBuffer[MAX_LEN];
static int rec;
static int sock;
pthread_t udpThreadId;

void ListenUDP_cleanupUDPThread()
{
    close(sock);
}

static void initSocket()
{
    // Create the socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    // Clear the server address structure and populate it
    memset((char *)&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }
}

// Recieves Request
static char *RecieveRequest(char *str)
{
    unsigned int clilen = sizeof(cliAddr);
    memset(udpBuffer, 0, MAX_LEN);
    rec = recvfrom(sock, udpBuffer, MAX_LEN - 1, 0, (struct sockaddr *)&cliAddr, &clilen);
    if (rec < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }

    strncpy(str, udpBuffer, rec);
    str[rec] = 0;

    return str;
}

// Sends response to UDP
static int sendResponse(char *response)
{
    int clilen = sizeof(cliAddr);
    rec = sendto(sock, response, strlen(response), 0, (struct sockaddr *)&cliAddr, clilen);
    if (rec < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    return 0;
}

// Sends response + current stats of the program
static void formatAndSendResponse(char *response, bool success)
{

    char data[MAX_LEN];

    int writtenCharCount = sprintf(data, "{\"response_text\": \"%s\", \"success\": %d, \"motion\": %d, \"frameNum\": %d}\n", response, success, Camera_getMotionState(), Camera_getFrameNum());
    data[writtenCharCount] = 0;
    sendResponse(data);
}

// Waits and listen for next request, and then handles it
static void listenForNextReq(bool isFirstBeat)
{

    bool success = false;
    char *response;
    char request[MAX_LEN];
    RecieveRequest(request);
    
    if (strcmp(request, "ping") == 0)
    {
        response = "Recieved Ping!";
        success = true;
    } else if (strcmp(request, "togglealarm") == 0) {
        Buzzer_toggleAlarm();
        response = "Alarm toggled";
        success = true;
    }

    formatAndSendResponse(response, success);
    listenForNextReq(false);
}

void *udpThread(void *arg){
    initSocket();
    listenForNextReq(true);
    pthread_exit(NULL);
}

void ListenUDP_initUDPThread()
{
    pthread_create(&udpThreadId, NULL, &udpThread, NULL);
}