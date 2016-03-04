#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>

void SIGINTHandler(int);
int globalPid;

int fork_shell(char **args)
{
  pid_t pid, wpid,sid;
  int status;
  pid = fork();
  // Child process to run code
  if (pid == 0) {
    execvp(args[0], args);
  } else if (pid < 0) {
    // Error forking
    printf("Error Forking\n");
  } else {

  }
  return pid;
}

int make_tokenlist(char *buf, char *dataFields[])
{

  char input_line[1024];
  char *line;
  int i,n;

  i = 0;
  line =   buf;
  dataFields[i] = strtok(line, " ");
  do  {
    i++;
    line = NULL;
    dataFields[i] = strtok(line, " ");
  } while(dataFields[i] != NULL);

  return i;
}

void  SIGINTHandler(int sig)
{
  signal(sig, SIG_IGN);
  //  char *killPid;
  //  snprintf(killPid, sizeof killPid, "%s%d", "sudo kill -9 ", globalPid);
  //  char *args[] = {killPid, (char*)NULL};
  //  execvp(args[0], args);
  exit(0);
}

int main(int argc, char **argv)
{
  char buffer[1024], *dataFields[60];
  char *inputFilePath;
  char actualpath [4092];
  inputFilePath = realpath(argv[1], actualpath);

  int pidInt;
  pidInt = fork_shell(&argv[1]);
  globalPid = pidInt;

  char str[1024];
  snprintf(str, sizeof str, "%s%d%s", "/proc/", pidInt , "/stat");
  signal(SIGINT, SIGINTHandler);
  while(1){
    FILE *statFile = NULL;
    statFile = fopen(str, "r");
    fread(buffer, 500, 1, statFile);
    int n = make_tokenlist(buffer, dataFields);
    // printf("%s\n",dataFields[13]);
    // printf("%s\n",dataFields[14]);

    int l = atol(dataFields[13]);
    int m = atol(dataFields[14]);
    printf("utime = %ld\r\n", l/sysconf(_SC_CLK_TCK));
    printf("stime = %ld\r\n",  m/sysconf(_SC_CLK_TCK));
    sleep(1);
    system("clear");
  }

  //
  // double user_util = 100 * (utime_after - utime_before) / (time_total_after - time_total_before);
  // double sys_util = 100 * (stime_after - stime_before) / (time_total_after - time_total_before);

  return 0;
}
