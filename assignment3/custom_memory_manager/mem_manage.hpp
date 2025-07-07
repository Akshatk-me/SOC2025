#pragma once
#include <cstddef>
#include <sys/types.h>

#define POOLSIZE 2048

// Complex
class Complex {
public:
  Complex(double a, double b) : r(a), c(b) {}
  // parameter name is optional when doing function declaration e.g. (size_t)
  void *operator new(size_t);
  void *operator new[](size_t);
  void operator delete(void *);
  void operator delete[](void *);

private:
  double r; // Real Part
  double c; // Complex Part
};

// Memory Manager class

class IMemoryManager {
public:
  // Any class with one or more pure virtual functions is an abstract
  // class. Like it exists just to provide a basic structure for derived classes
  // to implement on The "=0" syntax is used to declare a function as purely
  // virtual
  virtual ~IMemoryManager(); // Destructor for this base class
  virtual void *allocate(size_t) = 0;
  virtual void free(void *) = 0;
};

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
