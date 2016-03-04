/*
 *  This code shows tha tou can pass the scheduling policy at the cammand line and then use the sched_setscheduler function to set the scheduling policy
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sched.h>
#include <sys/types.h>
#include <errno.h>
#include <math.h>
#include <omp.h>

#define KNUM 100000

int IOBound(void){

  int i,j, is_prime;
  int s;
  FILE* fp;

  fp = fopen("scanfiles", "w");
  i = 1;

  while (i < KNUM) {
        system ("od scanfiles > t");
        i++;
  }
}
int main(int argc, char **argv)
{

	pid_t pid;
    int i;

    int policy;
    struct sched_param param;

    if (argc != 2){
       perror("incorrect arguments");
       exit(0);
    }

    if (!strcmp(argv[1], "SCHED_OTHER")) {
         policy = SCHED_OTHER;
    }
    else if(!strcmp(argv[1], "SCHED_FIFO")){
         policy = SCHED_FIFO;
    }
    else if (!strcmp(argv[1], "SCHED_RR")){
         policy = SCHED_RR;
    }
    else{
         perror("Scheduling policy not understood\n");
         exit(EXIT_FAILURE);
    }

    /* set process to max priority for given scheduler */
     param.sched_priority = sched_get_priority_max(policy);

     if(sched_setscheduler(0,policy,&param)){
         perror("Error setting scheduler policy");
         exit(EXIT_FAILURE);
     }
	   while(1)
	   {
			     IOBound();
           sleep(1);

	   }

   return 0;
}
