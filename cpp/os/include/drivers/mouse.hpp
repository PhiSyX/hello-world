#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__

#include "drivers/driver.hpp"
#include "hardware/interrupts.hpp"
#include "hardware/port.hpp"
#include "types.hpp"

class MouseEventHandler
{
public:
  MouseEventHandler();

public:
  virtual void on_activate() const;
  virtual void on_mousedown(u8 button) const;
  virtual void on_mouseup(u8 button) const;
  virtual void on_mousemove(i32 x, i32 y) const;
};

class MouseDriver
  : public InterruptHandler
  , public Driver
{
  Port8Bit dataport;
  Port8Bit commandport;

  u8 buffer[3];
  u8 offset;
  u8 buttons;

  MouseEventHandler* handler;

public:
  MouseDriver(InterruptManager* manager, MouseEventHandler* handler);
  ~MouseDriver();

public:
  virtual u32 handle_interrupt(u32 esp);
  virtual void activate();
};

#endif
