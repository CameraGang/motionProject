#include "recordBuffer.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "helper.h"

pthread_t tid;
pthread_mutex_t saveMutex;

typedef struct
{
  unsigned char *data;
  int size;
} frame_t;

frame_t *buffer;
int bi = 0; // buffer index. Points to next empty frame

int motionStart = -1;
int motionEnd = -1;

// increments Bi, and loops it back to 0 if outside of buffer size
static void incrementBi()
{
  bi++;

  if (bi == BUFFER_SIZE)
  {
    bi = 0;
  }
}

// inits all the stuff needed
static void init()
{
  buffer = malloc(sizeof(frame_t) * BUFFER_SIZE);
  pthread_mutex_init(&saveMutex, NULL);
}

static void cleanup()
{
  free(buffer); // TODO free all frameNum inside
}

void RecordBuffer_recordFrame(unsigned char *frame, int size)
{
  free(buffer[bi].data);
  buffer[bi].data = malloc(sizeof(unsigned char) * size);
  memcpy(buffer[bi].data, frame, size);
  buffer[bi].size = size;
  incrementBi();
}

static void *saveToFileThread()
{
  pthread_mutex_lock(&saveMutex);
  fprintf(stderr, "------------------------------------------------> start: %d, end: %d\n", motionStart, motionEnd);
  char filename[100];
  snprintf(filename, 100, OUTPUT_FILE, (int)time(NULL));

  char mjpegPath[200];
  snprintf(mjpegPath, 200, "%s.mjpeg", filename);
  FILE *fp = fopen(mjpegPath, "w+b");

  // int length = motionEnd - motionStart;

  for (int i = motionStart; i < motionEnd; ++i)
  {
    fwrite(buffer[i].data, buffer[i].size, 1, fp);
  }

  pthread_mutex_unlock(&saveMutex);

  char conversionCommand[500];
  snprintf(conversionCommand, 500, "ffmpeg -r 5 -i %s.mjpeg -c copy %s.avi", filename, filename);
  char output[20];
  runCommand(conversionCommand, output);

  // snprintf(conversionCommand, 500, "rm %s.mjpeg", filename);

  // runCommand(conversionCommand, output);
  fclose(fp);

  pthread_exit(NULL);
}

void RecordBuffer_noteMotionStart()
{
  pthread_mutex_lock(&saveMutex);
  motionStart = bi;
  pthread_mutex_unlock(&saveMutex);
}

void RecordBuffer_noteMotionEnd()
{
  pthread_mutex_lock(&saveMutex);
  motionEnd = bi;
  pthread_mutex_unlock(&saveMutex);
  pthread_create(&tid, NULL, *saveToFileThread, NULL);
}

void RecordBuffer_begin()
{
  init();
}

void RecordBuffer_end()
{
  cleanup();
}