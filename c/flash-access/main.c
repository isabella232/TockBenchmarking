/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <unistd.h>
#include <perf.h>

void _start();
void _etext();
#define NUMBER_OF_SAMPLES 100
#define RUN_LENGTH (0x2000 - 0x100)
#define START_ADDRESS ((uint32_t *)_start)
#define END_ADDRESS ((uint32_t *)(_etext))
#define RANDOM_OFFSET_WIDTH 0x80

#define READ_AND_INCREMENT(x) tmp = *(x++); \
        asm volatile("#nothing" \
                 : "=r"(tmp) \
                 : "r"(tmp)); 

#define READ_AND_INCREMENT_X4(x) READ_AND_INCREMENT(x) \
        READ_AND_INCREMENT(x) \
        READ_AND_INCREMENT(x) \
        READ_AND_INCREMENT(x)

#define READ_AND_INCREMENT_X16(x) READ_AND_INCREMENT_X4(x) \
        READ_AND_INCREMENT_X4(x) \
        READ_AND_INCREMENT_X4(x) \
        READ_AND_INCREMENT_X4(x) 

void flash_bandwith_test(void);

volatile int never_optimize = 0;

int main(void)
{
  flash_bandwith_test();
  return 0;
}

void flash_bandwith_test(void)
{
  unsigned big_sum = 0;

  printf("### RESULTS ###\n");
  printf("benchmark: flash_bandwidth\n");
  printf("description: measures flash bandwidth reading\n");
  printf("Run length: %d\n", RUN_LENGTH);
  printf("run, cycles, cycles / byte\n");

  for (size_t i = 0; i < NUMBER_OF_SAMPLES; ++i)
  {
    uint32_t sum = 0;
    //  = START_ADDRESS;
    unsigned cycles1 = perf_cycles();
    for (uint32_t *start_ptr = START_ADDRESS; start_ptr < (START_ADDRESS + RUN_LENGTH);)
    {
      uint32_t tmp;
      READ_AND_INCREMENT_X16(start_ptr);
    }
    unsigned cycles2 = perf_cycles();
    unsigned time = cycles2 - cycles1;
    big_sum += sum;
    int cycles_per_byte = time / (RUN_LENGTH);
    printf("%u, %u, %d\n", i, time, cycles_per_byte);
  }
  printf("Got sum %d\n\n", big_sum);

  big_sum = 0;

  printf("### RESULTS ###\n");
  printf("benchmark: flash_random_bandwidth\n");
  printf("description: measures flash bandwidth reading width random offsets\n");
  printf("Max Offset: %d\n", RANDOM_OFFSET_WIDTH);
  printf("run, cycles, bytes_read, cycles / byte\n");

  for (size_t i = 0; i < NUMBER_OF_SAMPLES; ++i)
  {
    uint32_t sum = 0;
    uint32_t count = 0;
    uint32_t *start_ptr = START_ADDRESS + i % RANDOM_OFFSET_WIDTH;
    unsigned cycles1 = perf_cycles();
    while (start_ptr < END_ADDRESS)
    {
      uint32_t new_val = *start_ptr;
      sum += new_val;
      start_ptr += (new_val % RANDOM_OFFSET_WIDTH) + 1;
      count++;
    }
    unsigned cycles2 = perf_cycles();
    unsigned time = cycles2 - cycles1;
    big_sum += sum;

    int cycles_per_byte = time / (count * 4);
    printf("%u, %u, %lu, %d\n", i, time, count * 4, cycles_per_byte);
  }

  printf("Got sum %d\n\n", big_sum);
}
