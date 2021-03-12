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
#include <perf.h>
#include <stdio.h>

//#define DEBUG 1
volatile uint32_t VAL = 0xf0000000;
int main(void){
  unsigned start, end;

  printf("### RESULTS ###\n"
      "benchmark: measures loop overhead and\n"
      "           confirms a branch not taken predictor\n"
      "description: measures cycles for 1000 loops\n");

  printf("Test 1: empty loop\n");
  for(int i = 0; i < 32; i++){
    //simple loop
    //time this loop=branch taken + stall
    //=0
    //loop: (nothing), +=1, bne1000 taken
    start = perf_cycles();
    for(int j = 0; j < 1000; j++){
      // Prevent this loop from being optimized out
      asm volatile("");
    }
    end = perf_cycles();
    printf("\t%d,\t%d\n", i, end - start);
  }

  printf("Test 2: loop with always not taken inside\n");
  //time this loop: 1 branch not taken for each loop compared to above
  //time=(branch taken + stall) + and + maybe stall + branch not taken->no stall
  //=0, mv
  //loop: lw, addi, bnez not taken, bnez taken
  volatile int val = 0;//if val is 1 then
  for(int i = 0; i < 32; i++){
    start = perf_cycles();
    for(int j = 0; j < 1000; j++){
      if(val){//supposed to be not taken
        printf("not supposed to be taken\n");
      }
    }
    end = perf_cycles();
    printf("\t%d,\t%d\n", i, end - start);
  }

  printf("Test 3: alternating loop\n");
  for(int i = 0; i < 32; i++){
    //need to add loop with temp&val always taken and always not taken
    //need to make sure the instructions# are the same

    //simple alternating loop
    //time:
    //lw, mv, li, addi, sw, loop2, loop1
    //loop1: andi, bnez taken,     lw, addi, addi, sw, bne taken
    //loop2: andi, bnez not taken, lw, addi, addi, sw, bne taken
    volatile int temp;
    start = perf_cycles();
    for(int j = 0; j < 1000; j++){
      temp += (j & 1) ? 2 : 20;
    }
    end = perf_cycles();
    printf("\t%d,\t%d\n", i, end - start);
  }

  printf("Test 4: Always taken\n");
  for(int i = 0; i < 32; i++){
    /*
    //li, mv, j
    //lw, beqz taken, addi, beqz not taken
    volatile int temp = 0;
    start = perf_cycles();
    for(int j = 0; j < 1000; j++){
      //always taken
      if(!temp){
        asm volatile("");
      } else{
        asm volatile("");
        printf("Not supposed to be not taken\n");
      }
    }
    */

#ifdef __arm__
    printf("Arm: test4 not done\n");
    break;
#else
    int counter=3, temp=11;
    start = perf_cycles();
    asm ("#test 4 asm start");
    asm volatile("\
    li %0,12\n\
    li %1,1000\n\
    startloop:\n\
    bnez %0,skipped#skip addi-12\n\
    addi %0,%0,-12\n\
    skipped:\n\
    addi %1,%1,-1\n\
    bnez %1,startloop#back to bnez %0\
    "
    : "=r"(temp), "=r" (counter)
    );
    asm ("#test 4 asm end");
    end = perf_cycles();
    //printf("check%d,%d\n", counter, temp);
    printf("\t%d,\t%d\n", i, end - start);
#endif
  }

  return 0;
}
