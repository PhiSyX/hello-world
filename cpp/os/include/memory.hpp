#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__

#include "types.hpp"

struct MemoryChunk
{
  MemoryChunk* next;
  MemoryChunk* prev;
  bool allocated;
  usize size;
};

class MemoryManager
{

protected:
  MemoryChunk* first;

public:
  static MemoryManager* active_memory_manager;

  MemoryManager(usize first, usize size);
  ~MemoryManager();

public:
  void* malloc(usize size);
  void free(void* ptr);
};

void*
operator new(unsigned size);
void*
operator new[](unsigned size);

void*
operator new(unsigned size, void* ptr);
void*
operator new[](unsigned size, void* ptr);

void
operator delete(void* ptr);
void
operator delete[](void* ptr);

#endif
