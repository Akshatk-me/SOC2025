#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int rc = fork();

  if (rc < 0) {
    // fork failed
    fprintf(stderr, "yamete! the fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // Here's the child only code
    char *lscommand[3];
    lscommand[0] = strdup("ls");
    lscommand[1] = strdup("-l");
    lscommand[2] = NULL;

    execvp(lscommand[0], lscommand);
    printf("this'll only run if execvp fails");
    // exit(0);
  } else {
    // Wait for the previous child to die
    // wait(NULL);
    wait(NULL);
    printf("here's the dad process: %d\n", (int)getpid());
  }
  return 0;
}
