#include "drivers/mouse.hpp"

void
printf(char*);

MouseEventHandler::MouseEventHandler() {}

void
MouseEventHandler::on_activate() const
{}

void
MouseEventHandler::on_mousedown(u8 button) const
{}

void
MouseEventHandler::on_mouseup(u8 button) const
{}

void
MouseEventHandler::on_mousemove(i32 x, i32 y) const
{}

MouseDriver::MouseDriver(InterruptManager* input_manager,
                         MouseEventHandler* $handler)
  : InterruptHandler(input_manager, 0x2C)
  , dataport(0x60)
  , commandport(0x64)
{
  handler = $handler;
}

MouseDriver::~MouseDriver() {}

void
MouseDriver::activate()
{
  offset = 0;
  buttons = 0;

  if (handler != 0) {
    handler->on_activate();
  }

  commandport.write(0xA8);
  commandport.write(0x20);

  u8 status = dataport.read() | 2;
  commandport.write(0x60);
  dataport.write(status);

  commandport.write(0xD4);
  dataport.write(0xF4);
  dataport.read();
}

u32
MouseDriver::handle_interrupt(u32 esp)
{
  u8 status = commandport.read();
  if (!(status & 0x20)) {
    return esp;
  }

  buffer[offset] = dataport.read();
  if (handler == 0) {
    return esp;
  }

  offset = (offset + 1) % 3;

  if (offset != 0) {
    return esp;
  }

  if (buffer[1] != 0 || buffer[2] != 0) {
    handler->on_mousemove((i8)buffer[1], -((i8)buffer[2]));
  }

  for (u8 i = 0; i < 3; i++) {
    if ((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i))) {
      if (buttons & (0x1 << i)) {
        handler->on_mouseup(i + 1);
      } else {
        handler->on_mousedown(i + 1);
      }
    }
  }

  buttons = buffer[0];

  return esp;
}
