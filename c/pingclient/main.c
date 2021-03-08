#include <stdio.h>
#include <string.h>
#include <timer.h>

#include <ipc.h>
#include <perf.h>
#include <tock.h>

#define SAMPLE_COUNT 100

int ipc_svc_num = 0;

//or use alignas(64) unint32_t buf[..];
uint32_t buf[64/sizeof(uint32_t)] __attribute__((aligned(64)));

static void ipc_callback(__attribute__ ((unused)) int pid,
                          __attribute__ ((unused)) int len,
                          __attribute__ ((unused)) int buf_ptr,
                          __attribute__ ((unused)) void* ud) {
  //need to check if optimized out. Also what's the unused for?
}

int main(void) {
  //get magically existing server
  ipc_svc_num = ipc_discover("ipc.ping.server");

  if (ipc_svc_num < 0) {
    printf("No ipc service!\n");
    return -1;
  }
  //printf("Successfully discovered ipc num %d\n", ipc_svc_num);
  //printf("Sharing buffer at %x\n", (size_t)buf);

  //send to server, call callback when server replies
  ipc_register_client_cb(ipc_svc_num, ipc_callback, buf);

  //share buffer with other process. Where in server does it see this?
  ipc_share(ipc_svc_num, buf, 64);

  printf("### RESULTS ###1\n"
         "benchmark: measures ping time\n"
         "description: Measures cycles taken for one process to notify\n"
         "             the other and for the other to notify back.\n");

  printf("run, cycles\n");
  for(int i = 0; i < SAMPLE_COUNT; i++){
    unsigned start, end;
    start = perf_cycles();
    ipc_notify_svc(ipc_svc_num);
    yield();//won't schedule this process unless it's notified
    end = perf_cycles();
    printf("%d, %u\n", i, end-start);
  }

  printf("\n");
  return 0;
}

