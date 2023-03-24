#ifndef RECORD_BUFFER
#define RECORD_BUFFER

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>

#define BUFFER_SIZE 1024
#define OUTPUT_FILE "./rec_%d"        // extension will be added via code
#define OUTPUT_FILE_AV "./rec_%d.mp4" // extension will be added via code
void RecordBuffer_begin();
void RecordBuffer_end();
void RecordBuffer_recordFrame(unsigned char *frame, int size);
void RecordBuffer_noteMotionStart();
void RecordBuffer_noteMotionEnd();
void RecordBuffer_addFrame(AVPacket *frame);

#endif