#ifndef __SYSCALLS_HPP__
#define __SYSCALLS_HPP__

#include "hardware/interrupts.hpp"
#include "types.hpp"

class SyscallHandler : public InterruptHandler
{
public:
  SyscallHandler(InterruptManager* interrupt_manager, u8 interrupt_number);
  ~SyscallHandler();

public:
  virtual u32 handle_interrupt(u32 esp);
};

void
printf(char*);

#endif
