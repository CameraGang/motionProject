#include "camera.h"
#include "record.h"
#include <unistd.h>
#include "led.h"
#include "buzzer.h"
#include "ListenUDP.h"

int main()
{
  ListenUDP_initUDPThread();
  led_init();
  Buzzer_init();
  Camera_beginCamera();
  led_cleanup();
  Buzzer_cleanup();
  ListenUDP_cleanupUDPThread();
  return 0;
}
