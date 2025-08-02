#include <csr.h>
#include <uart.h>
#include <stdio.h>

volatile int* clear_interrupt14 = (volatile int*)0x8000388;
volatile int* clear_interrupt15 = (volatile int*)0x80003c8;

#if 0
void tqv_user_interrupt15(void) {
  uart_putc('\n');
  uart_putc('6');
  uart_putc('\n');
  *clear_interrupt = 1;
}
#else
void tqv_user_interrupt(int cause) {
  uart_putc((cause & 0xF) + '0' - 10);
  uart_putc('\n');
  if ((cause & 0xF) == 14) *clear_interrupt14 = 1;
  else *clear_interrupt15 = 1;
}
#endif

int main() {
  enable_interrupt(14);
  enable_interrupt(15);
  while (1) {
    int c = uart_getc();
    if (c != -1) {
      uart_putc(c);
    }
    if (uart_rx_interrupt_seen) {
      printf("\nInterrupt\n");
      uart_rx_interrupt_seen = 0;
    }
  }
  return 0;
}
