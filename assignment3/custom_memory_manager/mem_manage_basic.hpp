#pragma once
#include "mem_manage.hpp"

// Memory Manager class
class MemoryManager : public IMemoryManager {

  // Structure which will act as a pointer when not allocated a value
  // Else will be a pointer to the next free element in the pool

  struct FreeStore { // A named struct hence can act as a data type
    FreeStore *next;
  };

  void expandPoolSize();
  void cleanup();
  FreeStore *freeStoreHead; // freeStoreHead will contain the ptr to next memory

  // above this explicit mentioned public keyword is private
public:
  MemoryManager() { // Constructor
    freeStoreHead = 0;
    expandPoolSize();
  }

  // Destructor is virtual to ensure derived class
  // destructor is called when object is deleted
  // through base class pointer. clear the leftover free pool memory when
  // exiting
  virtual ~MemoryManager() { cleanup(); }

  virtual void *allocate(size_t); // this will serve as the new new()
  virtual void free(void *);
};
