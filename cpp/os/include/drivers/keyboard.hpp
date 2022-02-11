#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__

#include "driver.hpp"
#include "hardware/interrupts.hpp"
#include "hardware/port.hpp"
#include "types.hpp"

void
printf(char*);
void printh(u8);

class KeyboardEventHandler
{
public:
  KeyboardEventHandler();

public:
  virtual void on_keydown(char) const;
  virtual void on_keyup(char) const;
};

class KeyboardDriver
  : public InterruptHandler
  , public Driver
{
  Port8Bit data_port;
  Port8Bit command_port;

  KeyboardEventHandler* handler;

public:
  KeyboardDriver(InterruptManager* im, KeyboardEventHandler* handler);
  ~KeyboardDriver();

public:
  virtual u32 handle_interrupt(u32 esp);
  virtual void activate();
};

#endif
