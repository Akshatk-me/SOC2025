#pragma once
#include "mem_manage.hpp"

// Memory Manager class
class MemoryManager : public IMemoryManager {

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
