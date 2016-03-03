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


int fork_shell(char **args)
{
  pid_t pid, wpid,sid;
  int status;
  pid = fork();
  //printf("PID Parent:%d\n", pid);
  if (pid == 0) {
    /* Create a new SID for the child process */

    // if (sid < 0) {
    //   exit(EXIT_FAILURE);
    // }else{.
    sid = setsid();
    if(sid < 0)
    {
    exit(1);
    }
      printf("Child PID:%d\n", pid);
      system("/home/vagrant/CS3305/Asn2/cpuTimeWaste");
      umask(0);
      //set new session

      chdir("/");
      close(STDIN_FILENO);
      close(STDOUT_FILENO);
      close(STDERR_FILENO);
  } else if (pid < 0) {
    // Error forking
    printf("Error Forking\n");
  } else {

  }
  return pid;
}




int main(int argc, char **argv)
{

  char *inputFilePath;
  char actualpath [4092];
  inputFilePath = realpath(argv[1], actualpath);

  int pidInt;
  fork_shell(&argv[1]);
  sleep(3);
  //buffer[1024] = getpid(actualpath);
  printf("%s\n", inputFilePath );

  pid_t pid = strtoul(inputFilePath, NULL, 10);

  system(actualpath);
  //execvp("/bin/sh", argv[1]);
  printf("%l\n",pid);
  // if (fgets(input_line,MAX,stdin) != NULL){
  // }



  //
  // double user_util = 100 * (utime_after - utime_before) / (time_total_after - time_total_before);
  // double sys_util = 100 * (stime_after - stime_before) / (time_total_after - time_total_before);


}
