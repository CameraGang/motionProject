#ifndef RECORD
#define RECORD

typedef struct
{
  unsigned char *data;
  int size;
} frame_t;

void Record_addFrame(unsigned char *frameData, int size);

typedef struct
{
  int start;
  int end;
} motionMarkers_t;

void Record_markStart();
void Record_markEnd();
#endif