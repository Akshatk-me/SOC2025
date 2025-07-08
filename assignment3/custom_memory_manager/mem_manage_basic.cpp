#include "mem_manage.hpp"

void *MemoryManager::allocate(size_t size) {
  if (freeStoreHead == 0) {
    expandPoolSize();
  }

  FreeStore *head = freeStoreHead;
  freeStoreHead = head->next;
  return head;
}

// Set this deleted element's next pointer to current freeStoreHead
// change the current freeStoreHead to point to deleted element
// It was like somemem->freeStoreHead now it's
// somemem->deleted_mem->freeStoreHead
void MemoryManager::free(void *deleted) {
  FreeStore *head = static_cast<FreeStore *>(deleted);
  head->next = freeStoreHead;
  freeStoreHead = head;
}

// poolsize expansion code

void MemoryManager::expandPoolSize() {
  // allocate the size of each block in the pool to max between size of Complex
  // and size of FreeStore pointer
  size_t size = (sizeof(Complex) > sizeof(FreeStore *)) ? sizeof(Complex)
                                                        : sizeof(FreeStore *);
  // initiate the first element after current pool
  // set the freeStoreHead to the start of the expanded pool
  FreeStore *head = reinterpret_cast<FreeStore *>(new char[size]);
  freeStoreHead = head;

  // initialize the rest of the elements of the pool
  // these will be FreeStore* type storage elements which can obviously store
  // Complex type
  for (int i = 0; i < POOLSIZE; i++) {
    head->next = reinterpret_cast<FreeStore *>(new char[size]);
    head = head->next;
  }

  // set the last element's next pointer to 0.
  head->next = 0;
}

// cleanup code

void MemoryManager::cleanup() {

  FreeStore *nextPtr = freeStoreHead;

  // the initalization not done in for loop, it's done beforehand
  // hence the field is empty. As long as nextPtr isn't 0 the loop will continue
  // nextPtr will be updated to the next location to delete as pointed by
  // freeStoreHead which is updated every loop
  for (; nextPtr; nextPtr = freeStoreHead) {
    freeStoreHead = freeStoreHead->next;

    // since allocation of each element in expandPoolSize was done using new
    // char[size], this array's deallocation done using delete[].
    delete[] nextPtr;
  }
}

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
