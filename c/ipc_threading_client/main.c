#include <stdio.h>
#include <string.h>
#include <timer.h>

#include <ipc.h>
#include <perf.h>
#include <tock.h>

#define SAMPLE_COUNT 1000

int ipc_svc_num = 0;

uint32_t buf[64 / sizeof(uint32_t)];

uint32_t static_cycles;
size_t counter;

static void ipc_callback_inverse(__attribute__((unused)) int pid,
                         __attribute__((unused)) int len,
                         __attribute__((unused)) int buf_ptr, __attribute__((unused)) void *ud)
{
  uint32_t other_cycles = *buf;
  printf("%d, %ld\n", counter, other_cycles - static_cycles);
  counter++;
  if (counter < SAMPLE_COUNT)
  {
    static_cycles = perf_cycles();
    ipc_notify_svc(ipc_svc_num);
  }
}

static void ipc_callback(__attribute__((unused)) int pid,
                         __attribute__((unused)) int len,
                         __attribute__((unused)) int buf_ptr, __attribute__((unused)) void *ud)
{
  uint32_t cycles = perf_cycles();
  uint32_t other_cycles = *buf;
  printf("%d, %ld\n", counter, cycles - other_cycles);
  counter++;
  if (counter < SAMPLE_COUNT)
  {
    ipc_notify_svc(ipc_svc_num);
  }
  else
  {
    printf("### RESULTS ###\n");
    printf("benchmark:, measures ipc call overhead from client to server\n");
    printf("description:, Measures cycles taken for one process to notify the other\n");
    printf("run, cycles\n");
    counter = 0;
    ipc_register_client_cb(ipc_svc_num, ipc_callback_inverse, buf);
    ipc_notify_svc(ipc_svc_num);
  }
}

int main(void)
{
  printf("Starting!\n");
  counter = 0;
  ipc_svc_num = ipc_discover("ipc.profiling.server");

  if (ipc_svc_num < 0)
  {
    printf("No ipc service!\n");
    return -1;
  }
  printf("Successfully discovered ipc num %d\n", ipc_svc_num);
  printf("Sharing buffer at %x\n", (size_t)buf);

  ipc_register_client_cb(ipc_svc_num, ipc_callback, buf);
  ipc_share(ipc_svc_num, buf, sizeof(buf));

  printf("### RESULTS ###\n");
  printf("benchmark:, measures ipc call overhead from server to client\n");
  printf("description:, Measures cycles taken for one process to notify the other\n");
  printf("run, cycles\n");

  ipc_notify_svc(ipc_svc_num);
  return 0;
}
