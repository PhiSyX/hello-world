#ifndef __WIDGET_HPP__
#define __WIDGET_HPP__

#include "drivers/keyboard.hpp"
#include "drivers/vga.hpp"
#include "types.hpp"

class Widget : public KeyboardEventHandler
{
protected:
  const Widget* parent;

  i32 x;
  i32 y;
  i32 width;
  i32 height;

  u8 r;
  u8 g;
  u8 b;

  bool focussable;

public:
  Widget(const Widget* parent,
         const i32 x,
         const i32 y,
         const i32 w,
         const i32 h,
         const u8 r,
         const u8 g,
         const u8 b);
  ~Widget();

public:
  virtual void get_focus(const Widget* widget) const;
  virtual void model_to_screen(i32& x, i32& y) const;
  virtual const bool contains_coord(const i32 x, const i32 y) const;

  virtual void draw(VGA* gc);

  virtual void on_mousedown(const i32 x, const i32 y, const u8 button);
  virtual void on_mouseup(const i32 x, const i32 y, const u8 button);
  virtual void on_mousemove(const i32 old_x,
                            const i32 old_y,
                            const i32 new_x,
                            const i32 new_y);
};

class CompositeWidget : public Widget
{
private:
  isize total_children;
  Widget* children[100];
  const Widget* focussed_child;

public:
  CompositeWidget(const Widget* parent,
                  const i32 x,
                  const i32 y,
                  const i32 w,
                  const i32 h,
                  const u8 r,
                  const u8 g,
                  const u8 b);
  ~CompositeWidget();

public:
  virtual void get_focus(const Widget* widget);
  virtual bool add_child(Widget* child);

  virtual void draw(VGA* gc);
  virtual void on_mousedown(const i32 x, const i32 y, const u8 button);
  virtual void on_mouseup(const i32 x, const i32 y, const u8 button);
  virtual void on_mousemove(const i32 old_x,
                            const i32 old_y,
                            const i32 new_x,
                            const i32 new_y);

  virtual void on_keydown(const char);
  virtual void on_keyup(const char);
};

#endif
