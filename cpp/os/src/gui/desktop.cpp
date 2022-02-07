
#include "gui/desktop.hpp"

Desktop::Desktop(i32 w, i32 h, u8 r, u8 g, u8 b)
  : CompositeWidget(0, 0, 0, w, h, r, g, b)
  , MouseEventHandler()
{
  mousex = w / 2;
  mousey = h / 2;
}

Desktop::~Desktop() {}

void
Desktop::draw(VGA* gc)
{
  CompositeWidget::draw(gc);

  for (u8 i = 0; i < 4; i++) {
    gc->put_pixel(mousex - i, mousey, 0xFF, 0xFF, 0xFF);
    gc->put_pixel(mousex + i, mousey, 0xFF, 0xFF, 0xFF);
    gc->put_pixel(mousex, mousey - i, 0xFF, 0xFF, 0xFF);
    gc->put_pixel(mousex, mousey + i, 0xFF, 0xFF, 0xFF);
  }
}

void
Desktop::on_mousedown(u8 button)
{
  CompositeWidget::on_mousedown(mousex, mousey, button);
}

void
Desktop::on_mouseup(u8 button)
{
  CompositeWidget::on_mouseup(mousex, mousey, button);
}

void
Desktop::on_mousemove(i32 x, i32 y)
{
  x /= 4;
  y /= 4;

  i32 mouseX = mousex + x;
  if (mouseX < 0) {
    mouseX = 0;
  }
  if (mouseX >= width) {
    mouseX = width - 1;
  }

  i32 mouseY = mousey + y;
  if (mouseY < 0) {
    mouseY = 0;
  }
  if (mouseY >= height) {
    mouseY = height - 1;
  }

  CompositeWidget::on_mousemove(mousex, mousey, mouseX, mouseY);

  mousex = mouseX;
  mousey = mouseY;
}
