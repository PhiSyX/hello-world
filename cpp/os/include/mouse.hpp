#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__

#include "interrupts.hpp"
#include "port.hpp"
#include "types.hpp"

class MouseDriver : public InterruptHandler
{
  Port8Bit dataport;
  Port8Bit commandport;

  u8 buffer[3];
  u8 offset;
  u8 buttons;

  i8 x, y;

public:
  MouseDriver(InterruptManager* interrupt_manager);
  ~MouseDriver();

public:
  virtual u32 handle_interrupt(u32 esp);
};

#endif
