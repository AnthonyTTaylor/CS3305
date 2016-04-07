#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

typedef struct pageInfo{
  int frameNumber;
  int modified;
  int used;

}pageInfoEntry;


int globalFrames;
int globalFaults = 0;
int isvalueinarray(int val, pageInfoEntry *arr, int size);
void lru(pageInfoEntry *table, int currentframe);
void lfu(pageInfoEntry *table, int currentframe);

struct timeval currentTime;

int main(int argc, char **argv)
{

  int numberOfFrames;
  char fpname[100];
  char typeOfResult[100];

  if (argc != 4){
    perror("incorrect arguments");
    exit(0);
  }
  numberOfFrames = atoi(argv[1]);
  globalFrames = numberOfFrames;
  strcpy(fpname,argv[2]);
  strcpy(typeOfResult, argv[3]);

  FILE *fp;
  fp = fopen(fpname, "r");

  int pageTableSize=0; //number of lines in the file

  //Calculate the number of frames in teh file so we can determin when we are done
  int ch;
  while(!feof(fp))
  {
    ch = fgetc(fp);
    if(ch == '\n')
    {
      pageTableSize++;
    }
  }


  //Read the frames one by one
  int traceStorage[pageTableSize];
  int i = 0;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  rewind(fp);

  while ((read = getline(&line, &len, fp)) != -1) {
    traceStorage[i++] = atoi(line);
  }

  pageInfoEntry *pageTable;
  pageTable = (pageInfoEntry *)malloc(sizeof(pageInfoEntry) * numberOfFrames);
  for(i =0; i < numberOfFrames; i++)
  {
    pageTable[i].frameNumber = -1;
    pageTable[i].modified = 0;
    pageTable[i].used = 0;
  }


  //for each frame inside of traceStorage, loop through and store it to the pageTable, pageTable is teh size of number of frames example 4
  for (size_t i = 0; i < pageTableSize; i++) {
    int exists = isvalueinarray(traceStorage[i], pageTable, numberOfFrames);
    if (exists == -1) {
      globalFaults++;
      for (size_t j = 0; j < numberOfFrames; j++) {
        if (pageTable[j].used == 0) {
          //pageInfoEntry *entry = malloc(sizeof(pageInfoEntry));
          pageTable[i].frameNumber = traceStorage[i];
          pageTable[i].modified = gettimeofday(&currentTime, NULL);//(unsigned)time(NULL);
          pageTable[i].used++;
        }else
        if (!strcmp(typeOfResult, "lru")) {
          lru(pageTable, traceStorage[i]);
        }else
        lfu(pageTable, traceStorage[i]);
      }
    }
  }
  printf("%i\n", globalFaults);
  for (size_t i = 0; i < numberOfFrames; i++) {
    printf("%i\n", pageTable[i].frameNumber );
  }
}

int isvalueinarray(int val, pageInfoEntry *arr, int size){
  int i;
  for (i=0; i < size; i++) {
    pageInfoEntry *temp = &arr[i];
    if (arr[i].frameNumber == val)
    return i;
  }
  return -1;
}

// typedef struct pageInfo{
//   int frameNumber;
//   int modified;
//   int used;
//
// }pageInfoEntry;
//lease freq used
void lfu(pageInfoEntry *table, int currerntframe)
{
  int lu=0;
  int leastUsed = table[0].used;
  for(int i=1; i < globalFrames; i++)
  {
    if(table[i].used < leastUsed)
    {
      leastUsed = table[i].modified;
      lu = i;
    }
  }
  table[lu].frameNumber = currerntframe;
  table[lu].used = 1;
  table[lu].modified = gettimeofday(&currentTime, NULL);;//(unsigned)time(NULL);
}


// typedef struct pageInfo{
//   int frameNumber;
//   int modified;
//   int used;
//
// }pageInfoEntry;
void lru(pageInfoEntry *table, int currentframe)
{
  //replace value which was used least recently
  int o = 0;
  long double oldest = table[0].modified;
  for(int i=1; i < globalFrames; i++)
  {
    if(table[i].modified < oldest)
    {
      oldest = table[i].modified;
      o = i;
    }
  }
  table[o].frameNumber = currentframe;
  table[o].used = 1;
  table[o].modified = gettimeofday(&currentTime, NULL);//(unsigned)time(NULL);
}

