#include "syscalls.hpp"

SyscallHandler::SyscallHandler(InterruptManager* im, u8 in)
  : InterruptHandler(im, in + im->hardware_interrupt_offset)
{}

SyscallHandler::~SyscallHandler() {}

u32
SyscallHandler::handle_interrupt(u32 esp)
{
  CPUState* cpu = (CPUState*)esp;

  switch (cpu->eax) {
    case 4:
      printf((char*)cpu->ebx);
      break;

    default:
      break;
  }

  return esp;
}
