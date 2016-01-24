#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>


#define MAX_LENGTH 1024

char *command_line_parser(void)
{
return "hello";
}

//This forks
int shell_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      printf("error with child process\n");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    printf("Error Forking\n");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

// ls command
 char *listCurrentDirectory()
 {
    char cwd[50];
    getcwd(cwd, sizeof(cwd));
    printf("Current working dir: %s", cwd);
    DIR *mydir;
    struct dirent *myfile;
  //  struct stat mystat;

    char buf[512];
    mydir = opendir("/");
    while((myfile = readdir(mydir)) != NULL)
    {
        //sprintf(buf, "%s/%s", argv[1], myfile->d_name);
        //stat(buf, &mystat);
      //printf("%zu",mystat.st_size);
        printf(" %s ", myfile->d_name);
    }
    printf("\n");
    closedir(mydir);
}


int main(int argc, char *argv[]) {
//while(1){
 char * loginName = getlogin();
 printf("%s>", loginName);
 listCurrentDirectory();
  int i = 1;
  for (i = optind; i < argc; ++i)
  {
    //printf("%i>\n", getlogin_r);
    printf("%s\n", argv[i]);
    //password = argv[i+1];
    i++;
  }
//  }
  return 0;
}
