// Code referenced from https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2022-student-howtos/StreamingWebcamFromBeagleBoneToNodeJSServer.pdf
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define PORT_T 3000
#define RPORT_T 1234

static struct sockaddr_in sinT;
static struct sockaddr_in sinRemoteT;
static int socketDescriptorT;

void UDP_openConnection()
{
    memset(&sinT, 0, sizeof(sinT));
    sinT.sin_family = AF_INET;
    sinT.sin_addr.s_addr = htonl(INADDR_ANY);
    sinT.sin_port = htons(PORT_T);
    socketDescriptorT = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptorT, (struct sockaddr *)&sinT, sizeof(sinT));
    sinRemoteT.sin_family = AF_INET;
    sinRemoteT.sin_port = htons(RPORT_T);
    sinRemoteT.sin_addr.s_addr = inet_addr("192.168.7.1");
}
int UDP_sendFrame(const void *str, int size)
{
    int packetSent = 0;
    sendto(socketDescriptorT,
           str,
           size,
           0,
           (struct sockaddr *)&sinRemoteT,
           sizeof(sinRemoteT));
    return packetSent;
}
void UDP_closeConnection()
{
    close(socketDescriptorT);
}