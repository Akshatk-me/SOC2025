#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

// signal handler
volatile sig_atomic_t gSignalstatus;
static void signal_handler(int signal) {
  gSignalstatus = signal;
  printf("Received signal: %d\n", signal);
}

/* Splits the string by space and returns the array of tokens
 *
 */
char **tokenize(char *line) {
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for (i = 0; i < strlen(line); i++) {

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t') {
      token[tokenIndex] = '\0';
      if (tokenIndex != 0) {
        tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
        strcpy(tokens[tokenNo++], token);
        tokenIndex = 0;
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }

  free(token);
  tokens[tokenNo] = NULL;
  return tokens;
}

int execute(char **tokens) {
  int ws;
  int *proc_list = (int *)malloc(100 * sizeof(int));
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "Fork Failed!\n");
    exit(1);
  } else if (rc == 0) {
    int exec_status = execvp(tokens[0], tokens);
    if (exec_status < 0) {
      // printf("execl returned! errno is [%d]\n", errno);
      // perror("The error message is :");
      perror(NULL);
    }
    // printf("exec_status: %d\n", exec_status);
    exit(exec_status);
  } else {
    // enable the below if you don't want CTRL-C to close the shell
    // signal(SIGINT, signal_handler);
    wait(&ws);

    if (ws != 0) {
      printf("EXITSTATUS: %d\n", WEXITSTATUS(ws));
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  char line[MAX_INPUT_SIZE];
  char **tokens;
  char cd[] = "cd";
  char EXIT_CMD[] = "exit";
  int i;
  char *cwd = (char *)malloc(257 * sizeof(char));
  /*
   */

  while (1) {
    /* BEGIN: TAKING INPUT */
    bzero(line, sizeof(line));
    if (getcwd(cwd, 257) != NULL) {
      printf("%s", cwd);
    } else {
      printf("failed to get current dir\n");
      fprintf(stderr, "failed to get the current dir\n");
    }
    printf("$ ");
    scanf("%[^\n]", line);
    getchar(); // this is for waiting till enter is pressed
               // write some code which will handle CTRL-D i.e. stdin being
               // closed

    // printf("Command entered: %s (remove this debug output later)\n", line);
    /* END: TAKING INPUT */

    line[strlen(line)] = '\n'; // terminate with new line
    tokens = tokenize(line);
    /*
    for (i = 0; tokens[i] != NULL; i++) {
      printf("found token %s (remove this debug output later)\n", tokens[i]);
    } */

    if (tokens[0] == NULL) {
      // printf("you said null?\n");
    } else if ((strcmp(tokens[0], cd) == 0) && (tokens[1] != NULL)) {

      // change the directory
      if (chdir(tokens[1]) != -1) {
      } else {
        perror(NULL); // if error print it
      }

    } else if (strcmp(tokens[0], EXIT_CMD) == 0) {
      // terminate all background process

      // kill(__pid_t pid, SIGINT);

      // clean up internal state (free the allocated memory)
      // Memory will be cleaned up after this conditional statement is exited

    } else {
      execute(tokens);
    }

    // Freeing the allocated memory
    for (i = 0; tokens[i] != NULL; i++) {
      free(tokens[i]);
    }
    free(tokens);
    free(cwd);
  }

  return 0;
}
