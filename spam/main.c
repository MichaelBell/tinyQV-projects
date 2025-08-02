#include <csr.h>
#include <uart.h>

int main() {
  for (int i = 0; i < 1000; ++i) {
    uart_putc('a');
    uart_putc('b');
  }
  uart_putc('\r');
  return 0;
}
