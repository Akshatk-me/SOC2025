#include "mem_manage.hpp"

MemoryManager gMemoryManager; // global instance of memory manager

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
