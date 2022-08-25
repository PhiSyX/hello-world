#ifndef __MULTITASKING_HPP__
#define __MULTITASKING_HPP__

#include "gdt.hpp"
#include "types.hpp"

struct CPUState
{
  u32 eax;
  u32 ebx;
  u32 ecx;
  u32 edx;

  u32 esi;
  u32 edi;
  u32 ebp;

  u32 error;

  u32 eip;
  u32 cs;
  u32 eflags;
  u32 esp;
  u32 ss;
} __attribute__((packed));

class Task
{
  friend class TaskManager;

private:
  u8 stack[4096]; // 4 KiB
  CPUState* cpustate;

public:
  Task(const GlobalDescriptorTable* gdt, const void entrypoint());
  ~Task();
};

class TaskManager
{
private:
  mutable Task* tasks[256];
  usize total_task;
  mutable i32 current_task;

public:
  TaskManager();
  ~TaskManager();

public:
  bool add(Task* task);

  const CPUState* schedule(CPUState* cpustate) const;
};

#endif
