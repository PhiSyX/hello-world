#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include "gui/widget.hpp"
#include "types.hpp"

class Window : public CompositeWidget
{
protected:
  bool dragging;

public:
  Window(const Widget* parent,
         const i32 x,
         const i32 y,
         const i32 w,
         const i32 h,
         const u8 r,
         const u8 g,
         const u8 b);
  ~Window();

public:
  void on_mousedown(const i32 x, const i32 y, const u8 button);
  void on_mouseup(const i32 x, const i32 y, const u8 button);
  void on_mousemove(const i32 old_x,
                    const i32 old_y,
                    const i32 new_x,
                    const i32 new_y);
};

#endif
