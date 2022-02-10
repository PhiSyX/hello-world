#include "gui/widget.hpp"

Widget::Widget(Widget* $parent,
               i32 $x,
               i32 $y,
               i32 w,
               i32 h,
               u8 $r,
               u8 $g,
               u8 $b)
  : KeyboardEventHandler()
{
  parent = $parent;
  x = $x;
  y = $y;
  width = w;
  height = h;
  r = $r;
  g = $g;
  b = $b;
  focussable = true;
}

Widget::~Widget() {}

void
Widget::get_focus(Widget* widget) const
{
  if (parent != 0) {
    parent->get_focus(widget);
  }
}

void
Widget::model_to_screen(i32& $x, i32& $y) const
{
  if (parent != 0) {
    parent->model_to_screen($x, $y);
  }

  $x += x;
  $y += y;
}

void
Widget::draw(VGA* gc)
{
  i32 $x = 0;
  i32 $y = 0;

  model_to_screen($x, $y);
  gc->fill_rect($x, $y, width, height, r, g, b);
}

void
Widget::on_mousedown(i32 x, i32 y, u8 button)
{
  if (focussable) {
    get_focus(this);
  }
}

void
Widget::on_mouseup(i32 x, i32 y, u8 button)
{}

void
Widget::on_mousemove(i32 old_x, i32 old_y, i32 new_x, i32 new_y)
{}

const bool
Widget::contains_coord(i32 $x, i32 $y) const
{
  return x <= $x && $x < x + width && y <= $y && $y < y + height;
}

CompositeWidget::CompositeWidget(Widget* parent,
                                 i32 x,
                                 i32 y,
                                 i32 w,
                                 i32 h,
                                 u8 r,
                                 u8 g,
                                 u8 b)
  : Widget(parent, x, y, w, h, r, g, b)
{
  focussed_child = 0;
  total_children = 0;
}

CompositeWidget::~CompositeWidget() {}

void
CompositeWidget::get_focus(Widget* widget)
{
  focussed_child = widget;

  if (parent != 0) {
    parent->get_focus(this);
  }
}

bool
CompositeWidget::add_child(Widget* child)
{
  if (total_children >= 100) {
    return false;
  }

  children[total_children++] = child;
  return true;
}

void
CompositeWidget::draw(VGA* gc)
{
  Widget::draw(gc);
  for (usize i = total_children - 1; i >= 0; --i) {
    children[i]->draw(gc);
  }
}

void
CompositeWidget::on_mousedown(i32 $x, i32 $y, u8 button)
{
  for (usize i = 0; i < total_children; ++i)
    if (children[i]->contains_coord($x - x, $y - y)) {
      children[i]->on_mousedown($x - x, $y - y, button);
      break;
    }
}

void
CompositeWidget::on_mouseup(i32 $x, i32 $y, u8 button)
{
  for (usize i = 0; i < total_children; ++i)
    if (children[i]->contains_coord($x - x, $y - y)) {
      children[i]->on_mouseup($x - x, $y - y, button);
      break;
    }
}

void
CompositeWidget::on_mousemove(i32 old_x, i32 old_y, i32 new_x, i32 new_y)
{
  i32 first_child = -1;

  for (usize i = 0; i < total_children; ++i) {
    auto child = children[i];

    if (child->contains_coord(old_x - x, old_y - y)) {
      child->on_mousemove(old_x - x, old_y - y, new_x - x, new_y - y);
      first_child = i;
      break;
    }
  }

  for (usize i = 0; i < total_children; ++i) {
    auto child = children[i];

    if (child->contains_coord(new_x - x, new_y - y)) {
      if (first_child != i) {
        child->on_mousemove(old_x - x, old_y - y, new_x - x, new_y - y);
      }
      break;
    }
  }
}

void
CompositeWidget::on_keydown(char key)
{
  if (focussed_child != 0) {
    focussed_child->on_keydown(key);
  }
}

void
CompositeWidget::on_keyup(char key)
{
  if (focussed_child != 0) {
    focussed_child->on_keyup(key);
  }
}
