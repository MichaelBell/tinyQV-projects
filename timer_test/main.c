#include <csr.h>
#include <uart.h>
#define printf uart_printf
#include <timer.h>

void timer_callback(void*) {
  int mtime = get_mtime();
  set_alarm(2, timer_callback, NULL);
  printf("Timer @ %d\n", mtime);
}

int main() {
  set_alarm(1, timer_callback, NULL);
  
  while (1);
  return 0;
}
