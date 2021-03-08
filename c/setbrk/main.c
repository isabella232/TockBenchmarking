/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <unistd.h>
#include <perf.h>

#define NUMBER_OF_SAMPLES 1000
#define BREAK_STRIDE 0x8

void setbrk_test(void);

int main(void) {
  setbrk_test();
  return 0;
}

void setbrk_test(void)
{
  printf("### RESULTS ###\n");
  printf("benchmark: setbrk_overhead\n");
  printf("description: changes the heap break which requires an MPU update\n");
  printf("run, cycles, break, break_decimal\n");

  for(size_t i = 0; i < NUMBER_OF_SAMPLES; ++i) {
    unsigned cycles1 = perf_cycles();
    void* old_break = memop(1, BREAK_STRIDE);
    unsigned cycles2 = perf_cycles();
    unsigned time = cycles2 - cycles1;
	void *break_ = (uint8_t*)old_break + BREAK_STRIDE;
    printf("%u, %u, %p, %u\n", i, time, break_, (size_t)break_);
  }
}
