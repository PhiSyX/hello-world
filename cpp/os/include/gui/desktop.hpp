#ifndef __DESKTOP_HPP__
#define __DESKTOP_HPP__

#include "drivers/mouse.hpp"
#include "drivers/vga.hpp"
#include "gui/widget.hpp"
#include "types.hpp"

class Desktop
  : public CompositeWidget
  , public MouseEventHandler
{
protected:
  u32 mousex;
  u32 mousey;

public:
  Desktop(i32 w, i32 h, u8 r, u8 g, u8 b);
  ~Desktop();

public:
  void draw(VGA* gc);

  void on_mousedown(u8 button);
  void on_mouseup(u8 button);
  void on_mousemove(i32 x, i32 y);
};

#endif
