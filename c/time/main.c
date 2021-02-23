/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <perf.h>

void time_test(void);

int main(void) {
  time_test();
  return 0;
}

void time_test(void)
{
  printf("### RESULTS ###\n");
  printf("benchmark: time_overhead_cycles\n");
  printf("run, cycles\n");
  for(size_t i = 0; i < 1000; ++i) {
    unsigned cycles1 = perf_cycles();
    unsigned cycles2 = perf_cycles();
  	printf("%u, %u\n", i, cycles2 - cycles1);
  }
  printf("benchmark: time_overhead_instructions\n");
  printf("run, instructions\n");
  for(size_t i = 0; i < 1000; ++i) {
    unsigned instr1 = perf_instructions_retired();
    unsigned instr2 = perf_instructions_retired();
    printf("%u, %u\n", i, instr2 - instr1);
  }
}
