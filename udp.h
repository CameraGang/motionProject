#ifndef FRAME_TO_SERVER_H
#define FRAME_TO_SERVER_H

void UDP_openConnection();
int UDP_sendFrame(const void *str, int size);
void UDP_closeConnection();

#endif