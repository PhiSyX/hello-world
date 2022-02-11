#include "syscalls.hpp"

SyscallHandler::SyscallHandler(InterruptManager* im, const u8 in)
  : InterruptHandler(im, in + im->hardware_interrupt_offset)
{}

SyscallHandler::~SyscallHandler() {}

const u32
SyscallHandler::handle_interrupt(const u32 esp) const
{
  const CPUState* cpu = (CPUState*)esp;

  switch (cpu->eax) {
    case 4:
      printf((char*)cpu->ebx);
      break;

    default:
      break;
  }

  return esp;
}
