#include "mouse.hpp"

MouseDriver::MouseDriver(InterruptManager* manager)
  : InterruptHandler(manager, 0x2C)
  , dataport(0x60)
  , commandport(0x64)
{
  u16* video_memory = (u16*)0xb8000;

  offset = 0;
  buttons = 0;
  x = 40;
  y = 12;

  video_memory[80 * y + x] = (video_memory[80 * y + x] & 0x0F00) << 4 |
                             (video_memory[80 * y + x] & 0xF000) >> 4 |
                             (video_memory[80 * y + x] & 0x00FF);
  commandport.write(0xA8);
  commandport.write(0x20);

  u8 status = dataport.read() | 2;
  commandport.write(0x60);
  dataport.write(status);

  commandport.write(0xD4);
  dataport.write(0xF4);
  dataport.read();
}

MouseDriver::~MouseDriver() {}

u32
MouseDriver::handle_interrupt(u32 esp)
{
  u8 status = commandport.read();

  if (!(status & 0x20)) {
    return esp;
  }

  buffer[offset] = dataport.read();
  offset = (offset + 1) % 3;

  if (offset != 0) {
    return esp;
  }

  if (buffer[1] != 0 || buffer[2] != 0) {
    static u16* video_memory = (u16*)0xb8000;
    video_memory[80 * y + x] = (video_memory[80 * y + x] & 0x0F00) << 4 |
                               (video_memory[80 * y + x] & 0xF000) >> 4 |
                               (video_memory[80 * y + x] & 0x00FF);

    x += buffer[1];
    if (x >= 80) {
      x = 79;
    }
    if (x < 0) {
      x = 0;
    }

    y -= buffer[2];
    if (y >= 25) {
      y = 24;
    }
    if (y < 0) {
      y = 0;
    }

    video_memory[80 * y + x] = (video_memory[80 * y + x] & 0x0F00) << 4 |
                               (video_memory[80 * y + x] & 0xF000) >> 4 |
                               (video_memory[80 * y + x] & 0x00FF);
  }

  return esp;
}
