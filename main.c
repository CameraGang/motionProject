#include "camera.h"
#include "record.h"
#include <unistd.h>
#include "led.h"
#include "buzzer.h"

int main()
{
  led_init();
  Buzzer_init();
  Camera_beginCamera();
  led_cleanup();
  Buzzer_cleanup();
  return 0;
}
