#include "mem_manage.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  Complex *array[1000];

  for (int i = 0; i < 5000; i++) {
    for (int j = 0; j < 1000; j++) {
      array[j] = new Complex(i, j);
    }
    std::cout << "1000 done" << std::endl;
    for (int j = 0; j < 1000; j++) {
      delete array[j];
    }
  }

  return 0;
}
