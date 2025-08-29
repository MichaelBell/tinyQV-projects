#include <stddef.h>
#include <gpio.h>
#include <stdio.h>
#include <csr.h>

// VGA console base addess is 0x800_0340 (0x800_0000 + 0x40*13)
volatile uint8_t* vga_text = (volatile uint8_t*)0x8000340;

volatile uint8_t* vga_bgcol = (volatile uint8_t*)0x8000370;
volatile uint8_t* vga_tcol1 = (volatile uint8_t*)0x8000371;
volatile uint8_t* vga_tcol2 = (volatile uint8_t*)0x8000372;
volatile uint8_t* vga_status = (volatile uint8_t*)0x800037f;

int count = 0;

void tqv_user_interrupt13(void) {
  (void)*vga_status;  // Clear interrupt

  char buf[11];
  sprintf(buf, "%10d", count);
  for (int i = 0; i < 10; ++i) vga_text[i] = buf[i];

  if ((count & 0xf) == 0) {
    const char scroll_text[] = "    TinyQV VGA Console!";
    int text_len = sizeof(scroll_text) - 1;
    int idx = (count >> 4) % text_len;
    for (int i = 0; i < 10; ++i) {
      vga_text[20+i] = scroll_text[idx];
      if (++idx == text_len) idx = 0;
    }
  }

  ++count;
}

int main(void) {
  // Clear text
  for (int i = 0; i < 30; ++i) vga_text[i] = ' ';

  // Set all outputs to regular mode (not debug)
  set_debug_sel(0xff);

  // Set all outputs to peripheral 13
  for (int i = 0; i < 8; ++i) set_gpio_func(i, 13);

  // Enable interrupt
  enable_interrupt(13);

  // Loop forever - all further work will be done in the
  // interrupt.
  while(1);
  return 0;
}
