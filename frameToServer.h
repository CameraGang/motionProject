#ifndef FRAME_TO_SERVER_H
#define FRAME_TO_SERVER_H

void FrameToServer_openConnectionT();
int FrameToServer_sendResponseT(const void *str, int size);
void FrameToServer_closeConnectionT();

#endif