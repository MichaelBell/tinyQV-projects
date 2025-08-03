#include <stddef.h>
#include <timer.h>
#include <gpio.h>

// Baby VGA base addess is 0x800_0140 (0x800_0000 + 0x40*5)
volatile uint32_t* framebuffer = (volatile uint32_t*)0x8000140;

// Cellular automata rule and initial state
const int rule = 30;
uint32_t cell = 0x8000;

int count = 0;

void render() {
  // Move all lines up one
  for (int i = 0; i < 15; ++i) {
    framebuffer[i] = framebuffer[i+1];
  }
  // Put the current cellular automata state on the bottom line
  framebuffer[15] = cell;
}

// Advance the cellular automata state
void advance() {
  uint32_t new_cell = 0;
  if (rule & (1 << ((cell << 1) & 7))) new_cell = 1;
  for (int i = 0; i < 31; ++i)
    if (rule & (1 << ((cell >> i) & 7))) new_cell |= 1 << (i+1);

  cell = new_cell;
}

void clear() {
  //for (int i = 0; i < 16; ++i) framebuffer[i] = ~0u >> (2*i);
  for (int i = 0; i < 16; ++i) framebuffer[i] = 0;
}

void timer_callback(void*) {
  // Call this again in 200ms
  set_alarm(200, timer_callback, NULL);

  // Advance the cellular automata and render to the screen
  advance();
  render();

  // Every 200 iterations, reset
  if (++count == 200) {
    clear();
    cell = 0x8000;
    count = 0;
  }
}

int main(void) {
  // Clear the frame buffer before selecting the peripheral
  clear();

  // Set all outputs to regular mode (not debug)
  set_debug_sel(0xff);

  // Set all outputs to peripheral 5
  for (int i = 0; i < 8; ++i) set_gpio_func(i, 5);

  // Manually call the update timer callback, this will
  // then set the timer to call it back at regular intervals
  timer_callback(NULL);

  // Loop forever - all further work will be done in the
  // timer interrupt.
  while(1);
  return 0;
}
