#include "camera.h"
#include "recordBuffer.h"
#include <unistd.h>

int main()
{
  RecordBuffer_begin();
  beginCamera();
  sleep(1);
  RecordBuffer_end();
  return 0;
}
