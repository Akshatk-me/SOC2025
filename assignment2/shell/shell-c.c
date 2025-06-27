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

int pid;

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

int execute(char **tokens, int RunBackground) {
  int ws;
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "Fork Failed!\n");
    exit(1);
  } else if (rc == 0) {
    int exec_status = execvp(tokens[0], tokens);
    if (exec_status < 0) {
      // printf("execl returned! errno is [%d]\n", errno);
      perror(NULL);
    }
    printf("exec_status: %d\n", exec_status);
    exit(exec_status);
  } else {

    // enable the below if you don't want CTRL-C to close the shell
    // signal(SIGINT, signal_handler);

    // loop to kill any zombie process from foreground + the forked process
    while ((pid = waitpid(-1, &ws, WNOHANG)) > 0) {
      if (ws != 0) {
        printf("EXITSTATUS: %d\n", WEXITSTATUS(ws));
      }
    }

    if (RunBackground == 0) {

      // printf("WILL be waiting for %d\n", rc);
      wait(&ws);
      if (ws != 0) {
        printf("EXITSTATUS: %d\n", WEXITSTATUS(ws));
      }

      // printf("WAS be waiting for %d\n", rc);

    } else {
      // the below will take care to eliminate the child when it dies
      // signal(SIGCHLD, signal_handler);
      printf("Running in Background: %d\n", rc);
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  char line[MAX_INPUT_SIZE];
  char **tokens;
  int i;
  int ws;
  int NUM_TOKENS;
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

    for (i = 0; tokens[i] != NULL; i++) {
      // printf("found %d tokens\n", i + 1);
    }

    NUM_TOKENS = i; // from the above run for loop

    if (tokens[0] == NULL) {
      // printf("you said null?\n");
    } else if ((strcmp(tokens[0], "cd") == 0) && (tokens[1] != NULL)) {

      // change the directory
      if (chdir(tokens[1]) != -1) {
      } else {
        perror(NULL); // if error print it
      }

    } else if (strcmp(tokens[i - 1], "&") == 0) {
      // do background execution
      tokens[i - 1] = NULL;
      execute(tokens, 1);

    } else {
      // do normal foreground execution
      execute(tokens, 0);
    }

    // Freeing the allocated memory
    for (i = 0; tokens[i] != NULL; i++) {
      free(tokens[i]);
    }
    free(tokens);
  }

  free(cwd);
  return 0;
}
