#include <stdio.h>
#include <string.h>
#include <timer.h>
#include <stdalign.h>

#include <ipc.h>
#include <perf.h>
#include <tock.h>

#define SAMPLE_COUNT 100


int main(void) {
  printf("### RESULTS ###\n");
  printf("benchmark: measures IPC service finding overhead\n");
  printf("description: measures cycles taken for one process to find a service\n");
  printf("run, cycles\n");

  for (size_t counter = 0; counter < SAMPLE_COUNT; ++counter)
  {
    uint32_t start = perf_cycles();
    int val = ipc_discover("ipc.profiling.server");
    uint32_t end = perf_cycles();
    printf("%d, %ld\n", counter, end - start);
	if (val < 0)
		printf("BUG: we should have found a server!\n");
  }

  printf("benchmark: measures IPC service finding overhead\n");
  printf("description: measures instructions taken for one process to find a service\n");
  printf("run, instructions\n");

  for (size_t counter = 0; counter < SAMPLE_COUNT; ++counter)
  {
    uint32_t start = perf_instructions_retired();
    int val = ipc_discover("ipc.profiling.server");
    uint32_t end = perf_instructions_retired();
    printf("%d, %ld\n", counter, end - start);
	if (val < 0)
		printf("BUG: we should have found a server!\n");
  }

  printf("benchmark: measures IPC service finding failure overhead\n");
  printf("description: measures cycles taken for one process to fail to find a service\n");
  printf("run, cycles\n");

  for (size_t counter = 0; counter < SAMPLE_COUNT; ++counter)
  {
    uint32_t start = perf_cycles();
    int val = ipc_discover("ipc.profiling.server.bad");
    uint32_t end = perf_cycles();
    printf("%d, %ld\n", counter, end - start);
	if (val >= 0)
		printf("BUG: we should not have found a server!\n");
  }

  printf("benchmark: measures IPC service finding failure overhead\n");
  printf("description: measures instructions taken for one process to fail to find a service\n");
  printf("run, instructions\n");

  for (size_t counter = 0; counter < SAMPLE_COUNT; ++counter)
  {
    uint32_t start = perf_instructions_retired();
    int val = ipc_discover("ipc.profiling.server.bad");
    uint32_t end = perf_instructions_retired();
    printf("%d, %ld\n", counter, end - start);
	if (val >= 0)
		printf("BUG: we should not have found a server!\n");
  }

  return 0;
}
