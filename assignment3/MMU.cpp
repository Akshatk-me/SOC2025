#include "MMU.hpp"

Config cfg = {.addressWidth = 32,
              .pageSize = 4096,
              .pageTableLevels = 2,
              .bitsPerLevel = {10, 10}};

// &cf = cfg. so cf and cfg have the same pointer
Memory MemoryInstance = Memory(cfg);

void MMU::storeVal(uint64_t vaddr, uint64_t val) {
  // store value val at address vaddr
}

uint64_t MMU::loadVal(uint64_t vaddr) {
  // vaddr has some format, it's a
}
