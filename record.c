#include "record.h"

#include "helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <unistd.h>
#define BUFFER_SIZE 512
#define PRE_FRAMES 10          // Number of frames that should be added to video before the motion has started
#define OUTPUT_FILE "./rec_%d" // extension will be added via code

frame_t *buffer[BUFFER_SIZE] = {NULL};
int bufferIndex = 0;
int motionStartIndex = -1;
int motionEndIndex = -1;

// increments buffer index so that it loops the buffer
static void incrementBufferIndex()
{
  bufferIndex++;

  if (bufferIndex >= BUFFER_SIZE)
  {
    bufferIndex = 0;
  }
}

// saves video
static void *saveToFileThread(void *args)
{
  motionMarkers_t *markers = (motionMarkers_t *)args; // cast to correct type

  // Set filename
  char filename[100];
  snprintf(filename, 100, OUTPUT_FILE, (int)time(NULL));

  // Open file for mjpeg
  char mjpegPath[200];
  snprintf(mjpegPath, 200, "%s.mjpeg", filename);
  FILE *fp = fopen(mjpegPath, "w+b");

  // Write frames to mjpeg file
  int i = markers->start;
  while (i != markers->end - 1)
  {
    fwrite(buffer[i]->data, buffer[i]->size, 1, fp);

    i++;
    if (i >= BUFFER_SIZE)
    {
      i = 0;
    }
  }

  fclose(fp);

  // Convert mjpeg to .avi
  char conversionCommand[500];
  snprintf(conversionCommand, 500, "ffmpeg -r 5 -i %s.mjpeg -c copy %s.avi", filename, filename);
  char output[20];
  runCommand(conversionCommand, output);

  // Copy .avi file to USB drive
  char usbPath[200];
  snprintf(usbPath, 200, "/home/debian/usb-mount/");
  char copyCommand[500];
  snprintf(copyCommand, 500, "cp %s.avi %s", filename, usbPath);
  runCommand(copyCommand, output);

  // delete mjpeg file
  snprintf(conversionCommand, 500, "rm %s.mjpeg", filename);
  runCommand(conversionCommand, output);

  free(markers);
  pthread_exit(NULL);
}

// Notes when motion began
void Record_markStart()
{
  motionStartIndex = bufferIndex < PRE_FRAMES ? bufferIndex : bufferIndex - PRE_FRAMES; // add some footage from before motion, if possible
}

// Notes when motion has ended
void Record_markEnd()
{
  motionEndIndex = bufferIndex;

  // Get args ready
  motionMarkers_t *markers = malloc(sizeof(motionMarkers_t));
  markers->start = motionStartIndex;
  markers->end = motionEndIndex;

  pthread_t tid;
  pthread_create(&tid, NULL, *saveToFileThread, (void *)markers);
}

// Adds frame to buffer
void Record_addFrame(unsigned char *frameData, int size)
{
  // Free old frame
  if (buffer[bufferIndex] != NULL)
  {
    free(buffer[bufferIndex]->data);
    free(buffer[bufferIndex]);
    buffer[bufferIndex] = NULL;
  }

  // If buffer frame is not initied, init it.
  if (buffer[bufferIndex] == NULL)
  {
    buffer[bufferIndex] = malloc(sizeof(frame_t));
    buffer[bufferIndex]->data = malloc(sizeof(unsigned char *) * size);
  }

  // copy data
  memcpy(buffer[bufferIndex]->data, frameData, size);

  buffer[bufferIndex]->size = size;

  incrementBufferIndex();
}
