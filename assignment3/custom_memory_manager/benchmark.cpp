#include <chrono>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void run_and_time(const char *binary_path) {
  using namespace std::chrono;

  std::cout << "Running: " << binary_path << "\n";

  auto start = high_resolution_clock::now();

  pid_t pid = fork();
  if (pid == 0) {
    // Child process
    execl(binary_path, binary_path, nullptr);
    // If execl returns, an error occurred
    perror("execl failed");
    _exit(1);
  } else if (pid > 0) {
    // Parent process
    int status;
    waitpid(pid, &status, 0);
  } else {
    perror("fork failed");
    return;
  }

  auto end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(end - start);
  std::cout << "Total time: " << duration.count() << " μs\n\n";
}

int main() {
  run_and_time("./final");
  run_and_time("./baseline");
  return 0;
}
