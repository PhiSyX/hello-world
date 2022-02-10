#ifndef __SYSCALLS_HPP__
#define __SYSCALLS_HPP__

#include "hardware/interrupts.hpp"
#include "types.hpp"

void
printf(char*);

class SyscallHandler : public InterruptHandler
{
public:
  SyscallHandler(InterruptManager* interrupt_manager, u8 interrupt_number);
  ~SyscallHandler();

public:
  virtual const u32 handle_interrupt(u32 esp) const;
};

#endif
