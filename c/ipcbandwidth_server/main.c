#include <ipc.h>
#include <tock.h>
#include <perf.h>

uint32_t buffer[((1<<13) + (1<<11))/sizeof(uint32_t)];

static void ipc_callback(int pid, int len, int buf, __attribute__ ((unused)) void* ud) {

  memcpy(buffer, ((uint32_t*)buf), len);
  uint32_t cycles = perf_cycles();
  ((uint32_t*)buf)[0] = cycles;
  ipc_notify_client(pid);
}

int main(void) {
  ipc_register_svc(ipc_callback, NULL);
  return 0;
}

