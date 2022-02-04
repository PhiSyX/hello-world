
#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__

#include "interrupts.hpp"
#include "port.hpp"
#include "types.hpp"

class KeyboardDriver : public InterruptHandler
{
  Port8Bit dataport;
  Port8Bit commandport;

public:
  KeyboardDriver(InterruptManager* manager);
  ~KeyboardDriver();

public:
  virtual u32 handle_interrupt(u32 esp);
};

#endif
