#include <iostream>
#include <perf.h>

#define DEBUG 1


int main(){

  //how many bytes to jump by
  int dists[] = {16, 32, 64};
  char ar[64*500];
#if DEBUG
  printf("Array bounds: %p-%p\n", ar, &ar[64*500-1]);
#endif

  //call perfcycles syscall
  for(int i = 0; i < 3; i++){
    int** startp = (int**)ar;
    int jumpDist = dists[i]/sizeof(int*);

    //set up pointers
    for(int i = 0; i < 499; i++){
      *startp = (int*)(startp + jumpDist);//jumpDist*sizeof(int*) forward
#if DEBUG
      printf("dist: %ld\n", (long)(*startp) - (long)(startp));
#endif
      startp += jumpDist;
    }
    *startp = nullptr;
#if DEBUG
    printf("end ptr: %p\n", startp);
#endif

    //do test
    int start, end;
    int count = 0;
    startp = (int**)ar;
    start = perf_cycles();
    while(startp){
      startp = (int**)(*startp);
#if DEBUG
      count++;
#endif
    }
    end = perf_cycles();
    printf("Cycles: %d\n", end-start);
#if DEBUG
    printf("%d:\t%p\n", count, startp);
#endif
  }
}
