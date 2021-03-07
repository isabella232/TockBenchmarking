/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <unistd.h>
#include <perf.h>

void _start();
void _etext();
#define NUMBER_OF_SAMPLES 100
#define RUN_LENGTH (0x2000-0x100)
#define START_ADDRESS ((uint32_t*)_start)
#define END_ADDRESS ((uint32_t*)(_etext))
#define RANDOM_OFFSET_WIDTH 0x80

void flash_bandwith_test(void);

int main(void) {
  flash_bandwith_test();
  return 0;
}

void flash_bandwith_test(void)
{
  unsigned times[NUMBER_OF_SAMPLES];
  unsigned big_sum = 0;

  for(size_t i = 0; i < NUMBER_OF_SAMPLES; ++i) {
    uint32_t sum = 0;
    uint32_t* start_ptr = START_ADDRESS;
    unsigned cycles1 = perf_cycles();
    for(size_t k = 0; k < RUN_LENGTH; k++){
      sum += *(start_ptr++);
    }
    unsigned cycles2 = perf_cycles();
    times[i] = cycles2 - cycles1;
    big_sum += sum;
  }
  
  printf("Got sum %d\n\n", big_sum);

  printf("### RESULTS ###\n");
  printf("benchmark: flash_bandwidth\n");
  printf("description: measures flash bandwidth reading\n");
  printf("Run length: %d\n", RUN_LENGTH);
  printf("run, cycles\n");
  for(size_t i = 0; i < NUMBER_OF_SAMPLES; i++){
    printf("%u, %u\n", i, times[i]);
  }

  uint32_t counts[NUMBER_OF_SAMPLES];
  big_sum = 0;

  for(size_t i = 0; i < NUMBER_OF_SAMPLES; ++i) {
    uint32_t sum = 0;
    uint32_t count = 0;
    uint32_t* start_ptr = START_ADDRESS + i % RANDOM_OFFSET_WIDTH;
    unsigned cycles1 = perf_cycles();
    while(start_ptr < END_ADDRESS){
      uint32_t new_val = *start_ptr;
      sum += new_val;
      start_ptr += (new_val % RANDOM_OFFSET_WIDTH) + 1;
      count ++;
    }
    unsigned cycles2 = perf_cycles();
    times[i] = cycles2 - cycles1;
    big_sum += sum;
    counts[i] = count;
  }
  
  printf("Got sum %d\n\n", big_sum);

  printf("### RESULTS ###\n");
  printf("benchmark: flash_random_bandwidth\n");
  printf("description: measures flash bandwidth reading width random offsets\n");
  printf("Max Offset: %d\n", RANDOM_OFFSET_WIDTH);
  printf("run, cycles, bytes_read\n");
  for(size_t i = 0; i < NUMBER_OF_SAMPLES; i++){
    printf("%u, %u, %lu\n", i, times[i], counts[i] * 4);
  }
}
