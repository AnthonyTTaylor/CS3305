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
int fork_shell(char **args , int rd, int wd)
{
  pid_t pid, wpid;
  int status;
  pid = fork();
  if (pid == 0) {
    // Child process to run code
    if (rd != STDIN_FILENO){
      if(dup2(rd, STDIN_FILENO) != STDIN_FILENO){
        fprintf(stderr, "Error: failed to redirect standard input\n");
        return -1;
      }
    }

    if (wd != STDOUT_FILENO){
      printf("redirect stdout to %d.", wd);
      if(dup2(wd, STDOUT_FILENO) != STDOUT_FILENO){
        fprintf(stderr, "Error: failed to redirect standard output\n");
        return -1;
      }
    }
    execvp(args[0], args);
    fprintf (stderr,  "Commmand Not supported\n");
    abort ();

  } else if (pid < 0) {
    // Error forking
    printf("Error Forking\n");
  } else {
    //Parent process to run code
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
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

void clean(char **var) {
  int i = 0;
  while(var[i] != NULL) {
    var[i] = NULL;
    i++;
  }
}



int main(void)
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
      if (input_line[strlen(input_line) - 1] == '\n'){
        input_line[strlen(input_line) - 1] = '\0';
      }
      free(cmdHistory[currentCommand]);
      cmdHistory[currentCommand] = strdup(input_line);
      currentCommand = (currentCommand + 1);
      n= make_tokenlist(input_line, tokens);
    }
    else
    printf("huh?\n");

    for (i = 0; i < n; i++){
      if (strstr(tokens[i], "exit") != NULL) {
        exit(0);
      }else
      //user wants command history
      if (strstr(tokens[i], "history") != NULL) {
        int k = 0;
        while(cmdHistory[k] != NULL) {
          printf("%s\n",cmdHistory[k]);
          k++;
        }
      }
      break;
    }


    //check for | command
    int containsPipe = 0;
    for (size_t z = 0; z < n; z++) {
      if(strstr(tokens[z], "|") != NULL) {
        containsPipe = 1;
        }
      }
      if (containsPipe == 0) {
        fork_shell(tokens, 0, 0);
      }



    int pipe_index = 2;
      int rd = STDIN_FILENO;
      int wd = STDOUT_FILENO;
      int fds[2];
      if (pipe(fds) != 0) {
        fprintf(stderr, "Error: unable to pipe command '%s'\n", tokens[0]);
        return -1;
      }

      wd = fds[1]; /*file descriptor for the write end of the pipe*/

      // delete the pipe symbol and insert a null to terminate the
      // first command's argument list
      tokens[pipe_index] = NULL;
      tokens[4] = NULL;

      // run first command: read from STDIN and write to the pipe
      fork_shell(tokens, rd, wd);
      close(fds[1]);

      rd = fds[0];
      wd = STDOUT_FILENO;

      // run the second command: read from the pipe and write to STDOUT
      // the argument for this command starts at pipe_index+1
      fork_shell(tokens+pipe_index+1, rd, wd);

  }
}
