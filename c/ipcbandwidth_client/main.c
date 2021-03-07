#include <stdio.h>
#include <string.h>
#include <timer.h>
#include <stdalign.h>

#include <ipc.h>
#include <perf.h>
#include <tock.h>

#define SAMPLE_COUNT 100

int ipc_svc_num = 0;

uint32_t alignas(4) buf[((1<<13) + (1<<11))/sizeof(uint32_t)];

static void ipc_callback(__attribute__ ((unused)) int pid,
                          __attribute__ ((unused)) int len,
                          __attribute__ ((unused)) int buf_ptr, __attribute__ ((unused)) void* ud)
{}

int main(void) {
  ipc_svc_num = ipc_discover("ipc.profiling.server");

  printf("Starting!\n");
  if (ipc_svc_num < 0) {
    printf("No ipc service!\n");
    return -1;
  }
  printf("Successfully discovered ipc num %d\n", ipc_svc_num);
  printf("Sharing buffer at %x\n", (size_t)buf);

  ipc_register_client_cb(ipc_svc_num, ipc_callback, buf);

  printf("### RESULTS ###\n");
  printf("benchmark:, measures ipc call overhead\n");
  printf("description:, Measures cycles taken for one process to notify the other\n");
  printf("run, cycles\n");
  ipc_share(ipc_svc_num, buf, sizeof(buf));
  for (size_t counter = 0; counter < SAMPLE_COUNT; ++counter)
  {
    uint32_t start = perf_cycles();
    ipc_notify_svc(ipc_svc_num);
    yield();
    // We've received data here
    printf("%d, %ld\n", counter, buf[0] - start);
	buf[1] = 0;
  }
  buf[1] = 1;
  ipc_notify_svc(ipc_svc_num);
  yield();

  return 0;
}
