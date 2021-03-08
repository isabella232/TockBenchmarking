/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <unistd.h>
#include <perf.h>

#define NUMBER_OF_SAMPLES 10

volatile int zero_const = 0;
volatile uint8_t *jump_loc;
unsigned times[NUMBER_OF_SAMPLES];

void icache_test(int);

int main(void)
{
  printf("### RESULTS ###\n");
  printf("benchmark: icache-test\n");
  printf("description: measure instruction cache performance by increasing number of instructions in loop\n");
  printf("nop_bytes, run, cycles\n");
  for(int nop_bytes = 0; nop_bytes<=16 * 1024; nop_bytes+= 1024){
    icache_test(nop_bytes);
    for (size_t i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
      printf("%d, %u, %u\n", nop_bytes, i, times[i]);
    }
  }
  
  return 0;
}

#ifdef __arm__
void icache_test(int backjump)
{
	(void)backjump;
}
#else
void icache_test(int backjump)
{
  volatile size_t counter = 0;
  volatile unsigned start, end;
  jump_loc = ((uint8_t *)&&cleanup_start) - backjump;
  if (!zero_const)
  {
    goto loop;
  }
  else if (zero_const == 1)
  {
    goto cleanup_start;
  }

// 16KB of nops, each nop is 2 bytes
#pragma GCC unroll(16 * 1024 / 2)
  for (size_t i = 0; i < (16 * 1024 / 2); i++)
  {
    asm volatile("nop");
  }
cleanup_start:
  end = perf_cycles();
  times[counter] = end - start;
  counter++;
  if (counter >= NUMBER_OF_SAMPLES)
  {
    return;
  }
  goto loop;
loop:
  start = perf_cycles();

  asm volatile("jr  %1"
               : "=r"(jump_loc)
               : "r"(jump_loc));
  asm("#end asm");
}
#endif
