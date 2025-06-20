#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// the parent process is almost always printed first... maybe CPU scheduler
// keeps scheduler keeps scheduling main thread first seeing it has cache loaded
// and other reasons...

int main(int argc, char *argv[]) {
  int rc = fork();

  if (rc < 0) {
    // fork failed
    fprintf(stderr, "yamete! the fork failed\n");
    exit(1);

  } else if (rc == 0) {
    // printf("Watashi wa kodomo(child) desu PID: %d\n", (int)getpid());
    sleep(10); // sleep for 10 seconds
    exit(0);
  } else {
    printf("I'm the parent with PID: %d\n", (int)getpid());

    int kill_result = kill(rc, 9); // sigkill to kill the child
    printf("Killing of %d succesful? %d\n", rc,
           kill_result); //-1 due to child killing itself before parent can
                         // without the wait() it will return 0
  }
  return 0;
}
