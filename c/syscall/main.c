/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <perf.h>

void command_test(void);

int main(void) {
  command_test();
  return 0;
}

void command_test(void)
{
  printf("### RESULTS ###\n");
  printf("benchmark: system_call_cycles\n");
  printf("run, cycles\n");
  for(size_t i = 0; i < 1000; ++i) {
    unsigned cycles1 = perf_cycles();
    perf_cycles();
    unsigned cycles2 = perf_cycles();
  	printf("%u, %u\n", i, cycles2 - cycles1);
  }
  printf("benchmark: " "system_call_instructions\n");
  printf("run, instructions\n");
  for(size_t i = 0; i < 1000; ++i) {
    unsigned instr1 = perf_instructions_retired();
    perf_cycles();
    unsigned instr2 = perf_instructions_retired();
    printf("%u, %u\n", i, instr2 - instr1);
  }
}
