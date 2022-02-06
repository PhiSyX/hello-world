#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include "gui/widget.hpp"
#include "types.hpp"

class Window : public CompositeWidget
{
protected:
  bool dragging;

public:
  Window(Widget* parent, i32 x, i32 y, i32 w, i32 h, u8 r, u8 g, u8 b);
  ~Window();

public:
  void on_mousedown(i32 x, i32 y, u8 button);
  void on_mouseup(i32 x, i32 y, u8 button);
  void on_mousemove(i32 old_x, i32 old_y, i32 new_x, i32 new_y);
};

#endif
