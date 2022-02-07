#include "multitasking.hpp"

Task::Task(GlobalDescriptorTable* gdt, void entrypoint())
{
  cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));

  cpustate->eax = 0;
  cpustate->ebx = 0;
  cpustate->ecx = 0;
  cpustate->edx = 0;

  cpustate->esi = 0;
  cpustate->edi = 0;
  cpustate->ebp = 0;

  cpustate->eip = (u32)entrypoint;
  cpustate->cs = gdt->get_code_segment_selector();
  cpustate->eflags = 0x202;
}

Task::~Task() {}

TaskManager::TaskManager()
{
  total_task = 0;
  current_task = -1;
}

TaskManager::~TaskManager() {}

bool
TaskManager::add(Task* task)
{
  if (total_task >= 256) {
    return false;
  }

  tasks[total_task++] = task;

  return true;
}

CPUState*
TaskManager::schedule(CPUState* cpustate)
{
  if (total_task <= 0) {
    return cpustate;
  }

  if (current_task >= 0) {
    tasks[current_task]->cpustate = cpustate;
  }

  if (++current_task >= total_task) {
    current_task %= total_task;
  }

  return tasks[current_task]->cpustate;
}
