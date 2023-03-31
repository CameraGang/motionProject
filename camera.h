#ifndef CAMERA
#define CAMERA

#define WIDTH 720
#define HEIGHT 720
#define THRESHOLD 30
#define STOP_RECORDING_THRESHOLD 15

void Camera_beginCamera();
void Camera_stop();

int Camera_getFrameNum();
int Camera_getMotionState();

#endif