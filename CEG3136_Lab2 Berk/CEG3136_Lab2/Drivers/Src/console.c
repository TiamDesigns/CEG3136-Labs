// Console driver

#include "mcu.h"

// Here we define system calls to retarget the Standard C Library's output
// stream (stdout) to the Serial Wire Viewer (SWV) Instrumentation Trace
// Macrocell (ITM) Data Console in the STM32CubeIDE.

// This allows use of functions like printf() when the Debugger is running,
// without the complexities of configuring a Universal Asynchronous
// Receiver/Transmitter (UART) peripheral in the target MCU and a
// serial console on the host machine.

int _write(int file, char *ptr, int len)
{
  (void)file;
  int i;

  for (i = 0; i < len; i++)
    ITM_SendChar(*ptr++);

  return len;
}
