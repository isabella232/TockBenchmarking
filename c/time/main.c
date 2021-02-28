/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <perf.h>

#define NUMBER_OF_SAMPLES 1000

void time_test(void);

int main(void) {
  time_test();
  return 0;
}

void time_test(void)
{
  unsigned cycles[NUMBER_OF_SAMPLES];
  unsigned instructions[NUMBER_OF_SAMPLES];

  for(size_t i = 0; i < NUMBER_OF_SAMPLES; ++i) {
    unsigned cycles1 = perf_cycles();
    unsigned cycles2 = perf_cycles();
    cycles[i] = cycles2 - cycles1;
  }
  
  for(size_t i = 0; i < NUMBER_OF_SAMPLES; ++i) {
    unsigned instr1 = perf_instructions_retired();
    unsigned instr2 = perf_instructions_retired();
    instructions[i] = instr2 - instr1;
  }
  printf("### RESULTS ###\n");
  printf("benchmark: time_overhead_cycles\n");
  printf("run, cycles\n");
  for(size_t i = 0; i < NUMBER_OF_SAMPLES; i++){
    printf("%u, %u\n", i, cycles[i]);
  }

  printf("\n### RESULTS ###\n");
  printf("benchmark: time_overhead_instructions\n");
  printf("run, instructions\n");
  for(size_t i = 0; i < NUMBER_OF_SAMPLES; i++){
    printf("%u, %u\n", i, instructions[i]);
  }
}
