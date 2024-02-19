#include <stdbool.h>
#include <mul.h>

#include <uart.h>
#define printf uart_printf

#define MAX_PRIMES 128
int primes[MAX_PRIMES];
int num_primes = 0;

int main() {
  int p = 3;
  while (true) {
    bool is_prime = true;
    int i;
    for (i = 0; i < num_primes; ++i) {
      if (mul32x16(primes[i], primes[i]) > p) break;
      if (p % primes[i] == 0) {
        is_prime = false;
        break;
      }
    }
    if (i == MAX_PRIMES) break;
    if (is_prime) {
      if (num_primes < MAX_PRIMES)
        primes[num_primes++] = p;
      printf("%d ", p);
    }
    p += 2;
  }
  return 0;
}
