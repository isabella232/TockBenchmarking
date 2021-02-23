#include <perf.h>
/*
The CPU pipeline does not use a branch predictor, so there is some overheadfor 
every branch taken (there is no stalls in the pipeline added for a branch that
is not taken).
A simple loop
A forcefully alternating loop
More complicated loops to confirm the presence or absence of a branchpredictor
Large  loops  to  probe  for  the  existence  and  behavior  of  the
  instruction cache
*/

/*

comparing later ot other results.
Subtract out the loop time from other tests with loops

   */
#include <stdio.h>
#define CALC 1
//#define DEBUG 1
int main(){
  int temp = 5;


  //simple loop
  int test1cycles;
  int start, end;
  //time this loop=branch taken + stall
  //=0
  //loop: (nothing), +=1, bne1000 taken
#if CALC
  start = perf_cycles();
#endif
  for(int i = 0; i < 1000; i++){
    asm("");
  }
#if CALC
  end = perf_cycles();
  test1cycles = end-start;
  printf("Cycles1: %d\n", test1cycles);
#endif


  temp++;
  //time this loop: 1 branch not taken for each loop compared to above
  //time=(branch taken + stall) + and + maybe stall + branch not taken->no stall
  //=0,
  //loop: and, bne1 taken, +=1, bne1000 taken so twice as long?
#if CALC
  start = perf_cycles();
#endif
  for(int i = 0; i < 1000; i++){
    asm("");
    if(temp & 0xf0000000){//supposed to be not taken
      asm("");
      temp++;
      printf("not supposed to be taken\n");
    }
  }
#if CALC
  end = perf_cycles();
  test1cycles = end-start;
  printf("Cycles2: %d\n", test1cycles);
#endif

  temp = 5;
  //simple alternating loop
  //time:
  //=0,
  //loop1: and, bnez taken...
  //loop2:
#if CALC
  start = perf_cycles();
#endif
  for(int i = 0; i < 1000; i++){
    if(i & 1){//if odd
      temp += 1;
    } else{
      temp += 2;
    }
  }
#if CALC
  end = perf_cycles();
  test1cycles = end-start;
  printf("Cycles3: %d\n", test1cycles);
#endif
  return 0;

  //start = 
  for(int i = 0; i < 1000; i++){
      temp += 1;
  }
  //end = 

  temp = 5;

  //start =
  for(int i = 0; i < 500; i++){
    temp += 1;
    temp += temp;
  }
  //end =




  temp = 5;
  //more complicated I guess?
  for(int i = 0; i < 1000; i++){

  }

}
