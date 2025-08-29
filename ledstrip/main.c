#include <stddef.h>
#include <timer.h>
#include <gpio.h>
#include <uart.h>

volatile uint8_t* ledstrip = (volatile uint8_t*)0x8000420;

#define REG_CTRL 0
#define REG_G 2
#define REG_R 1
#define REG_B 3
#define REG_CHAR 4

uint8_t next_char = 33;

void timer_callback(void*) {
  set_alarm(200, timer_callback, NULL);
  ledstrip[REG_CHAR] = next_char - 1;
  while (ledstrip[REG_CTRL] == 0);
  ledstrip[REG_CHAR] = next_char | 0x80;
  uart_putc(next_char);

  if (++next_char == 127) next_char = 33;
}

int main(void) {
  uart_puts("Hello, world!");

  // Set all outputs to regular mode (not debug)
  set_debug_sel(0xff);

  // Set out1 to peripheral 18
  set_gpio_func(1, 18);

  // Set up a colour
  ledstrip[REG_R] = 10;
  ledstrip[REG_G] = 0;
  ledstrip[REG_B] = 10;

  // Manually call the update timer callback, this will
  // then set the timer to call it back at regular intervals
  timer_callback(NULL);

  // Loop forever - all further work will be done in the
  // timer interrupt.
  while(1);
  return 0;
}
