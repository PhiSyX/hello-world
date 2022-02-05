#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__

#include "driver.hpp"
#include "interrupts.hpp"
#include "port.hpp"
#include "types.hpp"

void
printf(char*);
void printh(u8);

class KeyboardEventHandler
{
public:
  KeyboardEventHandler();

public:
  virtual void on_keydown(char);
  virtual void on_keyup(char);
};

class KeyboardDriver
  : public InterruptHandler
  , public Driver
{
  Port8Bit dataport;
  Port8Bit commandport;

  KeyboardEventHandler* handler;

public:
  KeyboardDriver(InterruptManager* interrupt_manager,
                 KeyboardEventHandler* handler);
  ~KeyboardDriver();

public:
  virtual u32 handle_interrupt(u32 esp);
  virtual void activate();
};

#endif
