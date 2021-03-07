#include <ipc.h>
#include <tock.h>
#include <perf.h>

uint32_t buffer[((1<<13) + (1<<11))/sizeof(uint32_t)];
static void ipc_callback(int pid, int len, int buf, __attribute__ ((unused)) void* ud) {
  // I'd do this in main or not in a callback, but we want to measure just how
  // fast we can make this, which would be intercepting this here
  memcpy(buffer, ((uint32_t*)buf), len);
  ((uint32_t*)buf)[0] = perf_cycles();
  ipc_notify_client(pid);
  buffer[0] = pid;
}

int main(void) {
  ipc_register_svc(ipc_callback, NULL);

  bool done = false;
  while (!done)
  {
    yield();
	done = buffer[1];
  }
  ipc_notify_client(buffer[0]);
  return 0;
}
