#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  printf("hello (pid:%d)\n", (int)getpid());
  int rc = fork();
  if (rc < 0) {
    // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // child (new process)
    printf("I'm the child (pid:%d)\n", (int)getpid());
    char *myargs[20];

    myargs[0] = strdup("wc");
    myargs[1] = strdup("fork_example_2.c");
    myargs[2] = NULL; // marks the end off array

    execvp(myargs[0], myargs);
    printf("this will only print if execvp fails\n");

  } else {
    // parent goes down this path (main)
    int rc_wait =
        wait(NULL); // wait makes this thread wait for the child
                    // process to finish before executing the following content
                    // otherwise the following may get executed before child

    printf("I'm parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait,
           (int)getpid());
  }

  return 0;
}
