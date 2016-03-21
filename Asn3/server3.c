#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_MSG_SIZE 1024

sem_t mutex;
sem_t empty;
sem_t full;

int *threadArray;
int numberOfThreads, arraysize;

// void initQueue(queue);
// void enqueue(queue);
// void dequeue(queue);
// void doProcessing (void *);

typedef struct{
  int *Nodeq ;
  int first;
  int last;
  int count;
}queue;
/*
* This function receives receives two numbers from the client and sends the sum back
*/


void *doProcessing (void *workThread)
{
  int numbytes;
  char recvBuff[MAX_MSG_SIZE], sendBuff[MAX_MSG_SIZE];
  time_t ticks;
  int a, b, sum;
  int rv;
  queue *q = workThread;
  while(1){


    int connfd;
    sem_wait(&full);
    sem_wait(&mutex);
    connfd = dequeue(q);
    sem_post(&mutex);
    sem_post(&empty);


    memset(recvBuff, '0',sizeof(recvBuff));
    memset(sendBuff, '0', sizeof(sendBuff));

    /*receive data from the client*/
    numbytes = recv(connfd,recvBuff,sizeof(recvBuff)-1,0);
    if (numbytes == -1){
      perror("recv");
      exit(1);
    }
    recvBuff[numbytes] = '\0';

    /*Extract the two numbers */
    sscanf(recvBuff, "%d", &a);

    sum = a * 10;

    snprintf(sendBuff, sizeof(sendBuff), "%d", sum);
    send(connfd, sendBuff, strlen(sendBuff),0);
    close(connfd);
  }
  return NULL;
}


void initQueue(queue *Node)
{
  Node->Nodeq = (int*)malloc(sizeof(int)*arraysize);
  Node->first = 0;
  Node->last = arraysize-1;
  Node->count = 0;
}

void enqueue(queue *Node, int x)
{

  Node->last = (Node->last+1) % arraysize;
  Node->Nodeq[Node->last] = x;
  Node->count = Node->count + 1;

}

int dequeue(queue *Node)
{
  int x;
  x = Node->Nodeq[ Node->first ];
  Node->first = (Node->first+1) % arraysize;
  Node->count = Node->count - 1;

  return x;
}

int main(int argc, char *argv[])
{
  int listenfd = 0, connfd = 0, client_sock,c, socket_desc;
  struct sockaddr_in serv_addr, client;
  int rv;

  printf("%s %s %s\n", argv[1], argv[2], argv[3] );
  //Port number that will be used to listen on, number of threads, size of connection array
  if(argc != 4)
  {
    printf("\n incorrect number of arugments");
    return 1;
  }

  numberOfThreads = atoi(argv[2]);
  long taskids[numberOfThreads];
  pthread_t threads[numberOfThreads];

  arraysize = atoi(argv[3]);
  queue *q = malloc(sizeof(int) * arraysize);
  initQueue(q);
  sem_init(&mutex, 0, 1);
  sem_init(&empty, 0, arraysize);
  sem_init(&full,0,0);
  //rc = 0;

  /*Socket creation and binding*/
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd <  0) {
    perror("Error in socket creation");
    exit(1);
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  rv = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (rv <  0) {
    perror("Error in binding");
    exit(1);
  }


  listen(listenfd, 10);

  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);
  pthread_t thread_id;

  puts("Connection accepted");

  int i;
  for(i=0; i< numberOfThreads; i++)
  {
    if( pthread_create( &thread_id[i] , NULL ,  doProcessing,  (void*)q) < 0)
        {
            perror("could not create thread");
            return 1;
        }
  }
  puts("Handler assigned");

  if (client_sock < 0)
  {
    perror("accept failed");
    return 1;
  }


  while(1)
  {
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if(client_sock>0)
    {
      sem_wait(&empty);
      sem_wait(&mutex);
      enqueue(q, client_sock);
      sem_post(&mutex);
      sem_post(&full);
    }
  }
}
