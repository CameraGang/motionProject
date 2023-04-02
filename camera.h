#ifndef CAMERA
#define CAMERA

#define WIDTH 720  // frame width
#define HEIGHT 720 // frame height

#define THRESHOLD 30                // the pixel diff amount need to "indicate" a motion
#define STOP_RECORDING_THRESHOLD 15 // number of no motion frames after a motion frame that need to happen before recording is stopped

void Camera_beginCamera();
void Camera_stop();

int Camera_getFrameNum();
int Camera_getMotionState();

#endif