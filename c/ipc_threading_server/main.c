#include <ipc.h>
#include <tock.h>
#include <perf.h>

static void ipc_callback(int pid, __attribute__ ((unused)) int len, int buf, __attribute__ ((unused)) void* ud) {
  uint32_t cycles = perf_cycles();
  uint32_t* cycle_pos = (uint32_t*)buf;
  *cycle_pos = cycles;
  ipc_notify_client(pid);
}

int main(void) {
  ipc_register_svc(ipc_callback, NULL);
  return 0;
}

