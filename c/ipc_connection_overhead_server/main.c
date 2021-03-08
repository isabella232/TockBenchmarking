#include <ipc.h>
#include <tock.h>
#include <perf.h>

static void ipc_callback(int pid, int len, int buf, __attribute__ ((unused)) void* ud) {
	(void)pid;
	(void)len;
	(void)buf;
}

int main(void) {
  ipc_register_svc(ipc_callback, NULL);
  return 0;
}
