#include "Config.hpp"
#include "MMU.hpp"
#include "Memory.hpp"
#include <iostream>

Config cfg = {.addressWidth = 32,
              .pageSize = 4096,
              .pageTableLevels = 2,
              .bitsPerLevel = {10, 10}};

// &cf = cfg. so cf and cfg have the same pointer
Memory MemoryInstance = Memory(cfg);

class Complex {
public:
  Complex(double a, double b) : r(a), c(b) {}

private:
  double r; // Real Part
  double c; // Complex Part
};

int main(int argc, char *argv[]) {
  Complex *array[1000];
  for (int i = 0; i < 5000; i++) {
    for (int j = 0; j < 1000; j++) {
      array[j] = new Complex(i, j);
    }
    for (int j = 0; j < 1000; j++) {
      delete array[j];
    }
  }
  return 0;
}
