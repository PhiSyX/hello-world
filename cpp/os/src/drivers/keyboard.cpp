#include "drivers/keyboard.hpp"

KeyboardEventHandler::KeyboardEventHandler() {}

void
KeyboardEventHandler::on_keydown(char) const
{}

void
KeyboardEventHandler::on_keyup(char) const
{}

KeyboardDriver::KeyboardDriver(InterruptManager* input_manager,
                               KeyboardEventHandler* $handler)
  : InterruptHandler(input_manager, 0x21)
  , dataport(0x60)
  , commandport(0x64)
{
  handler = $handler;
}

KeyboardDriver::~KeyboardDriver() {}

void
KeyboardDriver::activate()
{
  while (commandport.read() & 0x1) {
    dataport.read();
  }

  commandport.write(0xAE);
  commandport.write(0x20);

  u8 status = (dataport.read() | 1) & ~0x10;
  commandport.write(0x60);
  dataport.write(status);
  dataport.write(0xF4);
}

u32
KeyboardDriver::handle_interrupt(u32 esp)
{
  u8 key = dataport.read();

  if (handler == 0) {
    return esp;
  }

  if (key < 0x80) {
    switch (key) {
      case 0x02:
        handler->on_keydown('1');
        break;
      case 0x03:
        handler->on_keydown('2');
        break;
      case 0x04:
        handler->on_keydown('3');
        break;
      case 0x05:
        handler->on_keydown('4');
        break;
      case 0x06:
        handler->on_keydown('5');
        break;
      case 0x07:
        handler->on_keydown('6');
        break;
      case 0x08:
        handler->on_keydown('7');
        break;
      case 0x09:
        handler->on_keydown('8');
        break;
      case 0x0A:
        handler->on_keydown('9');
        break;
      case 0x0B:
        handler->on_keydown('0');
        break;

      case 0x10:
        handler->on_keydown('q');
        break;
      case 0x11:
        handler->on_keydown('w');
        break;
      case 0x12:
        handler->on_keydown('e');
        break;
      case 0x13:
        handler->on_keydown('r');
        break;
      case 0x14:
        handler->on_keydown('t');
        break;
      case 0x15:
        handler->on_keydown('z');
        break;
      case 0x16:
        handler->on_keydown('u');
        break;
      case 0x17:
        handler->on_keydown('i');
        break;
      case 0x18:
        handler->on_keydown('o');
        break;
      case 0x19:
        handler->on_keydown('p');
        break;

      case 0x1E:
        handler->on_keydown('a');
        break;
      case 0x1F:
        handler->on_keydown('s');
        break;
      case 0x20:
        handler->on_keydown('d');
        break;
      case 0x21:
        handler->on_keydown('f');
        break;
      case 0x22:
        handler->on_keydown('g');
        break;
      case 0x23:
        handler->on_keydown('h');
        break;
      case 0x24:
        handler->on_keydown('j');
        break;
      case 0x25:
        handler->on_keydown('k');
        break;
      case 0x26:
        handler->on_keydown('l');
        break;

      case 0x2C:
        handler->on_keydown('y');
        break;
      case 0x2D:
        handler->on_keydown('x');
        break;
      case 0x2E:
        handler->on_keydown('c');
        break;
      case 0x2F:
        handler->on_keydown('v');
        break;
      case 0x30:
        handler->on_keydown('b');
        break;
      case 0x31:
        handler->on_keydown('n');
        break;
      case 0x32:
        handler->on_keydown('m');
        break;
      case 0x33:
        handler->on_keydown(',');
        break;
      case 0x34:
        handler->on_keydown('.');
        break;
      case 0x35:
        handler->on_keydown('-');
        break;

      case 0x1C:
        handler->on_keydown('\n');
        break;
      case 0x39:
        handler->on_keydown(' ');
        break;

      default: {
        printf("KEYBOARD 0x");
        printh(key);
        break;
      }
    }
  }

  return esp;
}
