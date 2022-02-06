#ifndef __WIDGET_HPP__
#define __WIDGET_HPP__

#include "drivers/keyboard.hpp"
#include "drivers/vga.hpp"
#include "types.hpp"

class Widget : public KeyboardEventHandler
{
protected:
  Widget* parent;

  i32 x;
  i32 y;
  i32 width;
  i32 height;

  u8 r;
  u8 g;
  u8 b;

  bool focussable;

public:
  Widget(Widget* parent, i32 x, i32 y, i32 w, i32 h, u8 r, u8 g, u8 b);
  ~Widget();

public:
  virtual void get_focus(Widget* widget);
  virtual void model_to_screen(i32& x, i32& y);
  virtual bool contains_coord(i32 x, i32 y);

  virtual void draw(VGA* gc);

  virtual void on_mousedown(i32 x, i32 y, u8 button);
  virtual void on_mouseup(i32 x, i32 y, u8 button);
  virtual void on_mousemove(i32 old_x, i32 old_y, i32 new_x, i32 new_y);
};

class CompositeWidget : public Widget
{
private:
  int total_children;
  Widget* children[100];
  Widget* focussed_child;

public:
  CompositeWidget(Widget* parent, i32 x, i32 y, i32 w, i32 h, u8 r, u8 g, u8 b);
  ~CompositeWidget();

public:
  virtual void get_focus(Widget* widget);
  virtual bool add_child(Widget* child);

  virtual void draw(VGA* gc);
  virtual void on_mousedown(i32 x, i32 y, u8 button);
  virtual void on_mouseup(i32 x, i32 y, u8 button);
  virtual void on_mousemove(i32 old_x, i32 old_y, i32 new_x, i32 new_y);

  virtual void on_keydown(char);
  virtual void on_keyup(char);
};

#endif
