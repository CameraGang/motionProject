#include <fcntl.h>
#include <stdlib.h>

#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

#include "camera.h"
#include "buzzer.h"
#include "led.h"

static char *devName;
static int fd = -1; // camera file

int frames = 0; // stores the number of frames the camera has seen so far

struct buffer
{
  void *start;
  size_t length;
};
struct buffer *buffers; // the buffers for the camera

static unsigned int n_buffers; // number of buffers used for mmap method

#define CLEAR(x) memset(&(x), 0, sizeof(x))

// Method extracted from https://www.kernel.org/doc/html/v4.11/media/uapi/v4l/capture.c.html and then modified to meet our needs
static int xioctl(int fh, int request, void *arg)
{
  int r;

  do
  {
    r = ioctl(fh, request, arg);
  } while (-1 == r);

  return r;
}

// Method extracted from https://www.kernel.org/doc/html/v4.11/media/uapi/v4l/capture.c.html and then modified to meet our needs
static void openDevice()
{
  fd = open(devName, O_RDWR | O_NONBLOCK, 0);

  if (fd == -1)
  {
    printf("Cannot Open Device\n");
    exit(EXIT_FAILURE);
  }
}

// Method extracted from https://www.kernel.org/doc/html/v4.11/media/uapi/v4l/capture.c.html and then modified to meet our needs
static void initDevice()
{
  struct v4l2_capability cap;
  struct v4l2_format fmt;

  CLEAR(cap);
  if (xioctl(fd, VIDIOC_QUERYCAP, &cap) == -1)
  {
    printf("Cap error\n");
    exit(EXIT_FAILURE);
  }

  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
  {
    printf("%s is not a video capture device\n", devName);
    exit(EXIT_FAILURE);
  }

  CLEAR(fmt);
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width = WIDTH;
  fmt.fmt.pix.height = HEIGHT;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
  fmt.fmt.pix.field = V4L2_FIELD_NONE;

  if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
  {
    printf("format couldn't be set\n");
    exit(EXIT_FAILURE);
  }

  // Init all the buffers
  struct v4l2_requestbuffers req;

  CLEAR(req);
  req.count = 4;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req))
  {
    printf("%s does not support mmap\n", devName);
    exit(EXIT_FAILURE);
  }

  if (req.count < 2)
  {
    printf("Insufficient buffer memory on %s\n", devName);
    exit(EXIT_FAILURE);
  }

  buffers = calloc(req.count, sizeof(*buffers));

  if (!buffers)
  {
    printf("Out of memory\n");
    exit(EXIT_FAILURE);
  }

  for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
  {
    struct v4l2_buffer buf;

    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = n_buffers;

    if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
      printf("VIDIOC_QUERYBUF\n");

    buffers[n_buffers].length = buf.length;
    buffers[n_buffers].start =
        mmap(NULL /* start anywhere */,
             buf.length,
             PROT_READ | PROT_WRITE /* required */,
             MAP_SHARED /* recommended */,
             fd, buf.m.offset);

    if (MAP_FAILED == buffers[n_buffers].start)
    {
      printf("mmap buffer error\n");
      exit(EXIT_FAILURE);
    }
  }
}

// Method extracted from https://www.kernel.org/doc/html/v4.11/media/uapi/v4l/capture.c.html and then modified to meet our needs
static void startCapturing()
{

  unsigned int i;
  enum v4l2_buf_type type;

  for (i = 0; i < n_buffers; ++i)
  {
    struct v4l2_buffer buf;

    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;

    if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
      exit(EXIT_FAILURE);
  }

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
    exit(EXIT_FAILURE);
}

uint8_t *prevFrameData = NULL;

// Method extracted from https://www.kernel.org/doc/html/v4.11/media/uapi/v4l/capture.c.html and then modified to meet our needs
static void processImage(unsigned char *p, int size)
{
  fwrite(p, size, 1, stdout);

  // Create MJPEG codec
  AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_MJPEG);

  // Create the context... whatever that means
  AVCodecContext *codecContext = avcodec_alloc_context3(codec);

  // Open codec
  if (avcodec_open2(codecContext, codec, NULL) < 0)
  {
    printf("avcodec_open2 error\n");
  }

  // Take the mjpeg buffer data and put it in packet
  AVPacket packet;
  av_init_packet(&packet);
  packet.data = p;
  packet.size = size;

  // Send packet to avcodec to it can decode with the provided codec
  if (avcodec_send_packet(codecContext, &packet) < 0)
  {
    // Handle error
    printf("send packet error\n");
  }

  // Alloc frame
  AVFrame *frame = av_frame_alloc();

  // Receive the frame from avocdec and now we have the decoded data!
  if (avcodec_receive_frame(codecContext, frame) < 0)
  {
    // error
    printf("receive frame error\n");
  }

  uint8_t *frameData = frame->data[0];                    // grab data from one channel, because we don't care for color. This channel should be red channel
  int frameDataSize = frame->linesize[0] * frame->height; // get size

  if (prevFrameData == NULL)
  {
    prevFrameData = malloc(sizeof(uint8_t) * frameDataSize); // if no previous frame, alloc space for storing previous frame
  }
  else
  {
    /*
    THE MOTION DECTION STEP. Essentially, it will check if 10% of the pixels differ by more than THRESHOLD amount of rgb value
    */
    int differentPixels = 0;
    for (int i = 0; i < frameDataSize; ++i)
    {
      uint8_t prevPixel = prevFrameData[i];
      uint8_t curPixel = frameData[i];

      int diff = (int)((int)curPixel) - ((int)prevPixel); // get different
      diff = diff < 0 ? diff * -1 : diff;                 // get abs

      // check diff
      if (diff > THRESHOLD)
      {
        differentPixels++;
      }
    }

    // if more than 10% of pixels are different then motion!
    if (differentPixels >= frameDataSize * 0.1)
    {
        fprintf(stderr, "MOTION!\n");
        Buzzer_init();
        led_init();
    }
    else
    {
      // printf("no motion.\n");
      led_cleanup();
      Buzzer_cleanup();
    }
  }

  // set current frame as prevFrame, so it can be prevFrame for the next frame.
  memcpy(prevFrameData, frameData, frameDataSize);

  // close stuff
  avcodec_free_context(&codecContext);
  av_frame_free(&frame);

  frames++; // increment the count of frames dealt with.

  fflush(stderr);
  fprintf(stderr, ".");
  fflush(stdout);
}

// Method extracted from https://www.kernel.org/doc/html/v4.11/media/uapi/v4l/capture.c.html and then modified to meet our needs
static int readFrame()
{

  struct v4l2_buffer buf;

  CLEAR(buf);

  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;

  if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf))
  {
    printf("VIDIOC_DQBUF\n");
    exit(EXIT_FAILURE);
  }

  assert(buf.index < n_buffers);

  processImage(buffers[buf.index].start, buf.bytesused);

  if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
    printf("VIDIOC_QBUF\n");

  return 1;
}

// Method extracted from https://www.kernel.org/doc/html/v4.11/media/uapi/v4l/capture.c.html and then modified to meet our needs
static void uninitDevice(void)
{
  unsigned int i;

  for (i = 0; i < n_buffers; ++i)
    if (-1 == munmap(buffers[i].start, buffers[i].length))
    {
      printf("munmap\n");
    }

  free(buffers);
}

// Method extracted from https://www.kernel.org/doc/html/v4.11/media/uapi/v4l/capture.c.html and then modified to meet our needs
static void closeDevice(void)
{
  if (-1 == close(fd))
    printf("close\n");

  fd = -1;
}

// Method extracted from https://www.kernel.org/doc/html/v4.11/media/uapi/v4l/capture.c.html and then modified to meet our needs
void beginCamera()
{
  devName = "/dev/video0";
  openDevice();
  initDevice();
  startCapturing();

  while (1)
  {
    for (;;)
    {
      fd_set fds;
      struct timeval tv;
      int r;

      FD_ZERO(&fds);
      FD_SET(fd, &fds);

      /* Timeout. */
      tv.tv_sec = 2;
      tv.tv_usec = 0;

      r = select(fd + 1, &fds, NULL, NULL, &tv);

      if (-1 == r)
      {
        printf("Error: -1 == r\n");
        exit(EXIT_FAILURE);
      }

      if (0 == r)
      {
        printf("Select timeout\n");
        exit(EXIT_FAILURE);
      }

      if (readFrame())
        break;
    }
  }

  uninitDevice();
  closeDevice();
}
