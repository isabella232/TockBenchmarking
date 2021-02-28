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
  unsigned times[NUMBER_OF_SAMPLES];
  void* breaks[NUMBER_OF_SAMPLES];

  for(size_t i = 0; i < NUMBER_OF_SAMPLES; ++i) {
    unsigned cycles1 = perf_cycles();
    void* old_break = memop(1, BREAK_STRIDE);
    unsigned cycles2 = perf_cycles();
    times[i] = cycles2 - cycles1;
    breaks[i] = (uint8_t*)old_break + BREAK_STRIDE;
  }
  
  printf("### RESULTS ###\n");
  printf("benchmark: setbrk_overhead\n");
  printf("description: changes the heap break which requires an MPU update\n");
  printf("run, cycles, break, break_decimal\n");
  for(size_t i = 0; i < NUMBER_OF_SAMPLES; i++){
    printf("%u, %u, %p, %u\n", i, times[i], breaks[i], (size_t)breaks[i]);
  }
}
