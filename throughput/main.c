#include <csr.h>
#include <uart.h>
#define printf uart_printf

void mix_test(void);
void load_testb(void);
void load_testh(void);
void load_test(void);
void load_test2(void);
void load_test4(void);
void store_testb(void);
void store_testh(void);
void store_test(void);
void store_test2(void);
void store_test4(void);

int main() {
  volatile int* test_ptr = (int*)0x1001000;
  *test_ptr++ = 0x12345678;
  *test_ptr++ = 0xa5a5a5a5;
  *test_ptr++ = 0xfedcba98;
  *test_ptr++ = 0x5a5a5a5a;

#if 1
  int start_cycle, end_cycle;
  start_cycle = read_cycle();
  load_testb();
  end_cycle = read_cycle();
  printf("Lbu %d\n", end_cycle - start_cycle);
  delay_us(1000);
  
  start_cycle = read_cycle();
  load_testh();
  end_cycle = read_cycle();
  printf("Lhu %d\n", end_cycle - start_cycle);
  delay_us(1000);

  start_cycle = read_cycle();
  load_test();
  end_cycle = read_cycle();
  printf("Lw %d\n", end_cycle - start_cycle);
  delay_us(1000);

  start_cycle = read_cycle();
  load_test2();
  end_cycle = read_cycle();
  printf("Lw2 %d\n", end_cycle - start_cycle);
  delay_us(1000);

  start_cycle = read_cycle();
  load_test4();
  end_cycle = read_cycle();
  printf("Lw4 %d\n", end_cycle - start_cycle);
  delay_us(1000);

  start_cycle = read_cycle();
  store_testb();
  end_cycle = read_cycle();
  printf("Sb %d\n", end_cycle - start_cycle);
  delay_us(1000);

  start_cycle = read_cycle();
  store_testh();
  end_cycle = read_cycle();
  printf("Sh %d\n", end_cycle - start_cycle);
  delay_us(1000);

  start_cycle = read_cycle();
  store_test();
  end_cycle = read_cycle();
  printf("Sw %d\n", end_cycle - start_cycle);
  delay_us(1000);

  start_cycle = read_cycle();
  store_test2();
  end_cycle = read_cycle();
  printf("Sw2 %d\n", end_cycle - start_cycle);
  delay_us(1000);

  start_cycle = read_cycle();
  store_test4();
  end_cycle = read_cycle();
  printf("Sw4 %d\n", end_cycle - start_cycle);
#else

  mix_test();
  load_test2();
  load_test4();
  uart_putc('\r');
#endif

  return 0;
}
