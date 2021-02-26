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

//weak means prefer to use some other definition instead
//if can't can't find another definition, use this
/*
__attribute__((weak)) unsigned perf_cycles(void) {
  printf("perf.h not found\n");
  return 0;
}*/

//#define DEBUG 1
volatile uint32_t VAL = 0xf0000000;
int main(void){
  int temp = 5;
  unsigned start, end;


  int results[5][32];
  int resultIdx = 0;
  for(int i = 0; i < 32; i++){
    //simple loop
    //time this loop=branch taken + stall
    //=0
    //loop: (nothing), +=1, bne1000 taken
    start = perf_cycles();
    for(int j = 0; j < 1000; j++){
      asm("");
    }
    end = perf_cycles();
    results[resultIdx][i] = end-start;
  }
  resultIdx++;

  temp++;
  //time this loop: 1 branch not taken for each loop compared to above
  //time=(branch taken + stall) + and + maybe stall + branch not taken->no stall
  //=0,
  //loop: and, bne1 taken, +=1, bne1000 taken so twice as long?
  int val = VAL;//if val is 1 then
  for(int i = 0; i < 32; i++){
    start = perf_cycles();
    for(int j = 0; j < 1000; j++){
      asm("");
      if(temp & val){//supposed to be not taken
        asm("");
        temp++;
        printf("not supposed to be taken\n");
      }
    }
    end = perf_cycles();
    results[resultIdx][i] = end-start;
  }
  resultIdx++;

  for(int i = 0; i < 32; i++){
    //need to add loop with temp&val always taken and always not taken
    //need to make sure the instructions# are the same

    //simple alternating loop
    //time:
    //=0,
    //loop1: and, bnez taken...
    //loop2:
    temp = 5;
    start = perf_cycles();
    for(int j = 0; j < 1000; j++){
      if(j & 1){//if odd
        temp += 1;
      } else{
        temp += 2;
      }
    }
    end = perf_cycles();
    results[resultIdx][i] = end-start;
  }
  resultIdx++;


  for(int i = 0; i < 32; i++){
    start = perf_cycles();
    for(int j = 0; j < 1000; j++){
      //always taken
      if(temp | val){
        asm("");
      } else{
        asm("");
        temp++;
        printf("Not supposed to be not taken\n");
      }
    }
    end = perf_cycles();
    results[resultIdx][i] = end-start;
  }
  resultIdx++;

  printf("### RESULTS ###\n"
      "benchmark: measures loop overhead and\n"
      "           confirms a branch not taken predictor\n"
      "description: measures cycles for 1000 loops\n");
  for(int i = 0; i < resultIdx; i++){
    printf("Test%d\n", i);
    for(int j = 0; j < 32; j++){
      printf("\t%d,\t%d\n", j, results[i][j]);
    }
  }

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
