#include "mem_manage.hpp"
#include <iostream>

MemoryManager gMemoryManager; // global instance of memory manager

void *MemoryManager::allocate(size_t size) {
  if (size == sizeof(Complex)) { // single element allocation
    std::unordered_set<BitMapEntry *>::iterator freeMapI =
        FreeMapEntries.begin();
    // iterator is only used to access the first element
    // cuz FreeMapEntries assumes first element has space
    // There is no other way to access set's first element than a iterator

    if (freeMapI != FreeMapEntries.end()) { // if FreeMapentries isn't empty
      BitMapEntry *mapEntry = *freeMapI;
      Complex *freeBlock = mapEntry->AllocateFirstFreeBlock(size);
      if (mapEntry->BlocksAvailable == 0) {
        FreeMapEntries.erase(mapEntry);
      }
      return freeBlock;
    } else { // if FreeMapEntries is empty get new chunk, allocate from there
      AllocateChunkAndInitBitMap();
      // update the FreeMapEntries to include the new chunk
      FreeMapEntries.insert(&(BitMapEntryList[BitMapEntryList.size() - 1]));

      // return the first free block from the new chunk
      return BitMapEntryList[BitMapEntryList.size() - 1].AllocateFirstFreeBlock(
          size);
    }
  } else {                              // array allocation
    if (ArrayMemoryList.empty()) {      // No ArrayMemory available to allocate
      return AllocateArrayMemory(size); // get new memory for the array
    } else {                            // ArrayMemory exists
      std::map<void *, ArrayMemoryInfo>::iterator infoI =
          ArrayMemoryList.begin(); // first element of ArrayMemoryList
      std::map<void *, ArrayMemoryInfo>::iterator infoEndI =
          ArrayMemoryList.end(); // Last element of ArrayMemoryList

      while (infoI != infoEndI) { // Array has free chunks
        //.second used to access the value i.e. ArrayMemoryInfo associated with
        // the infoI element
        ArrayMemoryInfo info = (*infoI).second;
        if (info.StartPosition != 0) {
          // checks if there already is an array in this chunk
          // if already, then move to the next element in the ArrayMemoryList
          continue;
        } else { // This is a fresh chunk where allocation is possible
                 // get the BitMapEntry associated with this chunk
          BitMapEntry *entry = &BitMapEntryList[info.MemoryPoolListIndex];
          // static cast from unsigned long to int
          if (entry->BlocksAvailable <
              static_cast<int>(size / sizeof(Complex))) {
            // array requested is bigger than free blocks in the chunk
            return AllocateArrayMemory(size); // allocate fresh somewhere else
          } else {
            // set array start position and size
            info.StartPosition = BIT_MAP_SIZE - entry->BlocksAvailable;
            info.Size = static_cast<int>(size / sizeof(Complex));

            // this computation for baseAddress is clever
            // baseAddress = Chunk_Start + numberofblocks*sizeof(block)
            // sizeof(block) is sizeof(Complex)
            // When adding to a pointer, addition is performed terms of size of
            // Complex so multiplication is implicit

            Complex *baseAddress =
                static_cast<Complex *>(
                    MemoryPoolList[info.MemoryPoolListIndex]) +
                info.StartPosition;

            ArrayMemoryList[baseAddress] = info;
            SetMultipleBlockBits(&info, false);
            return baseAddress;
          }
        }
      }
    }
  }
}

void MemoryManager::SetBitAndTrackAvailability(void *object, bool flag) {
  int i = BitMapEntryList.size() - 1;
  for (; i >= 0; i--) {

    BitMapEntry *BitMapElement = &BitMapEntryList[i];

    // check if this complex object lies between this Chunk start and Chunk end
    if ((BitMapElement->Head() <= object) &&
        ((static_cast<Complex *>(BitMapElement->Head()) + BIT_MAP_SIZE - 1) >=
         object)) {
      int position = static_cast<Complex *>(object) -
                     static_cast<Complex *>(BitMapElement->Head());
      BitMapElement->SetBit(position, flag);
      flag ? BitMapElement->BlocksAvailable++
           : BitMapElement->BlocksAvailable--;

      // Insert the BitMapElement into FreeMapEntries set if deallocation is
      // happening
      if ((flag == false) && (BitMapElement->BlocksAvailable == 1)) {
        FreeMapEntries.insert(BitMapElement);
      }
    }
  }
}

void MemoryManager::SetMultipleBlockBits(ArrayMemoryInfo *info, bool flag) {
  BitMapEntry *mapEntry = &BitMapEntryList[info->MemoryPoolListIndex];
  mapEntry->SetMultipleBits(info->StartPosition, flag, info->Size);
}

void MemoryManager::free(void *object) {
  // Check if object is not an array
  if (ArrayMemoryList.find(object) == ArrayMemoryList.end()) {
    SetBitAndTrackAvailability(object, true);
  } else {
    ArrayMemoryInfo *info = &ArrayMemoryList[object];
    SetMultipleBlockBits(info, true);
  }
}

void *MemoryManager::AllocateChunkAndInitBitMap() {

  BitMapEntry BitMapElement;
  Complex *ChunkStart =
      reinterpret_cast<Complex *>(new char[sizeof(Complex) * BIT_MAP_SIZE]);
  MemoryPoolList.push_back(ChunkStart);
  BitMapElement.Index = MemoryPoolList.size() - 1;
  BitMapEntryList.push_back(BitMapElement);
  return ChunkStart;
}

void *MemoryManager::AllocateArrayMemory(size_t size) {
  void *chunkAddress = AllocateChunkAndInitBitMap();
  ArrayMemoryInfo info;
  info.MemoryPoolListIndex = MemoryPoolList.size() - 1;
  info.StartPosition = 0;
  info.Size = size / sizeof(Complex);

  // Throw error if Array size requested is bigger than BitMapSize
  if (info.Size > BIT_MAP_SIZE) {
    throw std::runtime_error("Array too large for a single chunk");
  }

  ArrayMemoryList[chunkAddress] = info;
  SetMultipleBlockBits(&info, false);
  return chunkAddress;
}

std::vector<void *> MemoryManager::GetMemoryPoolList() {
  return MemoryPoolList;
}

void BitMapEntry::SetBit(int position, bool flag) {
  BitMap.set(position, flag);
}

void BitMapEntry::SetMultipleBits(int position, bool flag, int count) {
  for (int i = position; i < position + count; i++) {
    BitMap.set(i, flag);
  }
}

Complex *BitMapEntry::AllocateFirstFreeBlock(size_t size) {
  for (int i = 0; i < BIT_MAP_SIZE; i++) {
    if (BitMap[i] == true) {
      BitMap.set(i, false);
      BlocksAvailable--;
      Complex *addr = static_cast<Complex *>(Head()) + i;
      return addr;
    }
  }
  throw std::runtime_error(
      "Error: AllocateFirstFreeBlock requested but no free block found");
}

void *BitMapEntry::Head() { return gMemoryManager.GetMemoryPoolList()[Index]; }

// replace the Complex's new and delete
// the . is used when accessing a member function / variable of an instance
// :: is used when you have to specify the scope to which a  name belongs to
void *Complex::operator new(size_t size) {
  return gMemoryManager.allocate(size);
}

void Complex::operator delete(void *pointerToDelete) {
  return gMemoryManager.free(pointerToDelete);
}

void *Complex::operator new[](size_t size) {
  return gMemoryManager.allocate(size);
}

void Complex::operator delete[](void *pointerToDelete) {
  return gMemoryManager.free(pointerToDelete);
}
