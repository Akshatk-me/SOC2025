#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t gSignalstatus;

static void signal_handler(int signal) {
  gSignalstatus = signal;

  // printf("ello mate you say something\n");
  if (gSignalstatus == SIGINT) {
    printf("Recieved sigterm but I'm immune to it\n");
  }
}

int main(int argc, char *argv[]) {

  // just to mess around I've added fork to this script as well

  int rc = fork();
  signal(SIGINT, signal_handler);

  if (rc < 0) {
    // fork failed
    fprintf(stderr, "yamete! the fork failed\n");
    exit(1);

  } else if (rc == 0) {
    while (1) {
      // printf("I'm alive: %d\n", (int)getpid());
      // printf("I'm alive:\n");
    }
    exit(0);

  } else {
    printf("I'm the parent with PID: %d\n", (int)getpid());
    wait(NULL);
  }
  return 0;
}
