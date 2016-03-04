#include <stdio.h>
#include <stdlib.h>
#include <time.h>
clock_t start, end;
double cpu_time_used;

void funcCall()
{

}


int main(void)
{
start = clock();
funcCall();
end = clock();
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
printf("Time for function call :\n%.17f\n",cpu_time_used);

start = clock();
getpid();
end = clock();
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
printf("Time for system call :\n%.17f\n",cpu_time_used);


}
