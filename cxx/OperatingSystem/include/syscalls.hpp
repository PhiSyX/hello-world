#ifndef __SYSCALLS_HPP__
#define __SYSCALLS_HPP__

#include "hardware/interrupts.hpp"
#include "types.hpp"

void
printf(char*);

class SyscallHandler : public InterruptHandler
{
public:
  SyscallHandler(InterruptManager* im, const u8 in);
  ~SyscallHandler();

public:
  virtual const u32 handle_interrupt(const u32 esp) const;
};

#endif
