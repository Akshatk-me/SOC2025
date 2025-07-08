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

#if defined(USE_BASIC)
#include "mem_manage_basic.hpp"
#elif defined(USE_BITWISE)
#include "mem_manage_bitwise.hpp"
#else
#include "mem_manage_basic.hpp"
#endif
