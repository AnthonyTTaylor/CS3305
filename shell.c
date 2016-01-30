#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LENGTH 1024
#define MAX 256
#define CMD_MAX 10

//This forks
int shell_launch(char **args)
{
  printf("FORKED\n");
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process to run code
    // if (execvp(args[0], args) == -1) {
    //   printf("error with child process\n");
    // }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    printf("Error Forking\n");
  } else {
    // Parent process to run code
    // do {
    //   wpid = waitpid(pid, &status, WUNTRACED);
    // } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

// ls command
char *listCurrentDirectory()
{
  char cwd[50];
  getcwd(cwd, sizeof(cwd));
  DIR *mydir;
  struct dirent *myfile;

  char buf[512];
  mydir = opendir(cwd);
  while((myfile = readdir(mydir)) != NULL)
  {
    printf(" %s ", myfile->d_name);
  }
  printf("\n");
  closedir(mydir);
}


int make_tokenlist(char *buf, char *tokens[])
{

  char input_line[MAX];
  char *line;
  int i,n;

  i = 0;
  line =   buf;
  tokens[i] = strtok(line, " ");
  do  {
    i++;
    line = NULL;
    tokens[i] = strtok(line, " ");
  } while(tokens[i] != NULL);

  return i;
}



void main(void)
{

  char input_line[MAX], *tokens[CMD_MAX], *cmdHistory[10];
  int i, n, currentCommand = 0;
  for (i = 0; i < 10; i++){
    cmdHistory[i] = NULL;
  }


  while(1){
    char * loginName = getlogin();
    printf("%s>",loginName);
    if (fgets(input_line,MAX,stdin) != NULL){
      n= make_tokenlist(input_line, tokens);

      if (input_line[strlen(input_line) - 1] == '\n'){
        input_line[strlen(input_line) - 1] = '\0';

        /* NULL to `free` is a NO-OP */
        free(cmdHistory[currentCommand]);
        cmdHistory[currentCommand] = strdup(input_line);
        currentCommand = (currentCommand + 1);
      }
    }
    else
    printf("huh?\n");

    for (i = 0; i < n; i++){
      if (strstr(tokens[i], "ls") != NULL) {
        listCurrentDirectory();
        //shell_launch("ls");
      }else if (strstr(tokens[i], "exit") != NULL) {
        //user wants to exit the shell
        printf("Command History before closig\n");
        int k = 0;
        while(cmdHistory[k] != NULL) {

          printf("%s\n",cmdHistory[k]);
          k++;
        }
        exit(0);
      }
      //printf("extracted token is %s\n", tokens[i]);


    }
  }
}
