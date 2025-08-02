#pragma once
#include <cstdint>
#include <vector>

// Defines how virtual address space is divided
struct Config {
  int addressWidth;              // Total virtual address
  int pageSize;                  // Page size in bytes
  int pageTableLevels;           // Number of levels
  std::vector<int> bitsPerLevel; // Number of bits used per level of translation
};
