#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // don't use the below statement, it won't flush the buffer
  // without a `\n` character. Since fork duplicates the buffer
  // you'll see multiple prints of this string by each child process
  // printf("I'm the parent with PID: %d", (int)getpid());

  printf("I'm the parent with PID: %d\n", (int)getpid());
  fflush(stdout); // explicitly flushing stdout buffer

  int N = 2;
  if (argc != 1) {
    N = atoi(argv[1]); // you can provide an argument for N
  }

  for (int i = 0; i < N; i++) {
    int rc = fork();

    if (rc < 0) {
      // fork failed
      fprintf(stderr, "yamete! the fork failed\n");
      exit(1);
    } else if (rc == 0) {
      printf("PID: %d\n", (int)getpid());
      exit(0); // essential so that child process doesn't continue the for loop
    } else {
      // Wait for the previous child to die
      wait(NULL);
    }
  }
  return 0;
}
