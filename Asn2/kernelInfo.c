#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BS 1024

void main()
{

  //get the path of the file
  FILE *kernalFile;
  FILE *cpuFile;
  char buf[BS], buf2[BS];

  cpuFile = fopen("/proc/cpuinfo", "r");
  kernalFile = fopen("/proc/version", "r");


  //print its contents
  while(fgets(buf, BS-1, cpuFile)){
    if ((strstr(buf, "vendor_id") != NULL) || (strstr(buf, "model name") != NULL)) {
      printf("%s", buf);
    }
  }
  while(fgets(buf2, BS-1, kernalFile)){
    printf("%s", buf2);
  }

  fclose(cpuFile);
  fclose(kernalFile);

  //return 0;

}
