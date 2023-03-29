#include "camera.h"
#include "record.h"
#include <unistd.h>
#include "led.h"

int main()
{
  led_init();
  Camera_beginCamera();
  led_cleanup();

  return 0;
}
