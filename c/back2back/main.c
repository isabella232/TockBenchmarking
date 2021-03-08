#include <perf.h>
#include <stdio.h>
/*
The test follows the same design as described by the lmbench which essentially traverses a linked list.
For each round of tests, we construct a 500 node linked list with a set number of bytes between each dereference.
Then we record the number of cycles it takes to traverse the whole linked list.
*/
#define DEBUG 0

struct node
{
  struct node *next;
};

int main(void){
  //how many bytes to jump by
  int dists[] = {16, 32, 64};
  struct node ar[64*500/sizeof(struct node)];
#if DEBUG
  printf("Array bounds: %p-%p\n", ar, &ar[64*500/sizeof(struct node)-1]);
#endif

  //call perfcycles syscall
  for(int i = 0; i < 3; i++){//for each test in dists[]
    printf("### RESULTS ###\n"
      "benchmark: back-to-back memory access as described by lmbench and\n"
      "           confirms there is no data cache, distance %d\n"
      "description: back-to-back attempts to blow out the data cache\n"
      "             by accessing at the start of each possible block\n", dists[i]);
    int jumpDist = dists[i]/sizeof(struct node);

    for (size_t k = 0; k < 1000; ++k)
    {
      //set up pointers
      struct node* startp = &ar[0];
      for(int j = 0; j < 499; j++){
        startp->next = startp + jumpDist;
#if DEBUG
        printf("dist: %ld\n", (long)(startp->next) - (long)(startp));
#endif
        startp = startp->next;
      }
      startp->next = NULL;
#if DEBUG
      printf("end ptr: %p\n", startp->next);
      int count = 0;
#endif

      //do test
      unsigned start, end;
      startp = &ar[0];
      start = perf_cycles();
      while(startp){
        startp = startp->next;
#if DEBUG
        count++;
#endif
      }

      end = perf_cycles();
      printf("%d, %d\n", k, end-start);
#if DEBUG
      printf("%d:\t%p\n", count, startp);
#endif
    }
  }
}
