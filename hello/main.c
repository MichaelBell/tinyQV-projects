#include <uart.h>
#define printf uart_printf

int a = 1;

int main() {
  printf("Hello, world!\n");
  printf("Hello %d\n", a);
  a = 2;
  printf("Hello %d\n", a);
  return 0;
}
