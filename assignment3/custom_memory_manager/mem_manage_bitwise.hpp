#pragma once
#include "mem_manage.hpp"
#include <bitset>
#include <cstring>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>

#define BIT_MAP_SIZE 1000

// each BitMapEntry encodes for all blocks in a particular chunk
// BIT_MAP_SIZE is the total number of blocks in each chunk
struct BitMapEntry {
  int Index; // Index in the BitMapEntryList
  int BlocksAvailable;
  std::bitset<BIT_MAP_SIZE> BitMap; // each int element holds status of 32
                                    // blocks (int = 4bytes = 32bits)

public:
  BitMapEntry()
      : BlocksAvailable(BIT_MAP_SIZE) { // set available blocks as bit_map_size
    BitMap.set();                       // set all bits to 1
  }

  void SetBit(int position, bool flag);                     // set one bit
  void SetMultipleBits(int position, bool flag, int count); // set multiple bits

  // retrieves first free block of bitmap and does SetBit to false
  Complex *AllocateFirstFreeBlock(size_t size);
  void *Head();
};

struct ArrayMemoryInfo {
  int MemoryPoolListIndex; // Index in the MemoryPoolList i.e. Chunk list
  int StartPosition;       // Starting position of array in bitmap
  int Size;                // Size of array
};

// Memory Manager class
class MemoryManager : public IMemoryManager {
  // List of pointers to chunk beginning
  std::vector<void *> MemoryPoolList;
  // List of info about blocks in each chunk
  std::vector<BitMapEntry> BitMapEntryList;

  // each element in MemoryPool corresponds to a BitMapEntry

  // For chunks that have free element in their bitmap
  std::unordered_set<BitMapEntry *> FreeMapEntries;

  // For allocating arrays, storing their size, etc as well
  std::map<void *, ArrayMemoryInfo> ArrayMemoryList;

private:
  void *AllocateArrayMemory(size_t size);
  void *AllocateChunkAndInitBitMap();
  void SetBitAndTrackAvailability(void *object, bool flag);
  void SetMultipleBlockBits(ArrayMemoryInfo *info, bool flag);

public:
  MemoryManager() {}
  ~MemoryManager() {}
  virtual void *allocate(size_t);
  virtual void free(void *);
  std::vector<void *> GetMemoryPoolList();
};
