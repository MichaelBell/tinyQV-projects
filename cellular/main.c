#include <stddef.h>
#include <timer.h>
#include <gpio.h>

volatile uint32_t* framebuffer = (volatile uint32_t*)0x8000140;

const int rule = 30;
uint32_t cell = 0x8000;

int count = 0;

void render() {
  for (int i = 0; i < 15; ++i) {
    framebuffer[i] = framebuffer[i+1];
  }
  framebuffer[15] = cell;
}

void advance() {
  uint32_t new_cell = 0;
  if (rule & (1 << ((cell << 1) & 7))) new_cell = 1;
  for (int i = 0; i < 31; ++i)
    if (rule & (1 << ((cell >> i) & 7))) new_cell |= 1 << (i+1);

  cell = new_cell;
}

void clear() {
  for (int i = 0; i < 16; ++i) framebuffer[i] = ~0u >> (2*i);
}

void timer_callback(void*) {
  set_alarm(200, timer_callback, NULL);
  advance();
  render();
  if (++count == 200) {
    clear();
    cell = 0x8000;
    count = 0;
  }
}

int main(void) {
  clear();
  set_debug_sel(0xff);
  for (int i = 0; i < 8; ++i) set_gpio_func(i, 5);
  //timer_callback(NULL);
  while(1);
  return 0;
}
