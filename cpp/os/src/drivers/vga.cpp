#include "drivers/vga.hpp"

VGA::VGA()
  : misc_port(0x3c2)
  , crtc_index_port(0x3d4)
  , crtc_data_port(0x3d5)
  , sequencer_index_port(0x3c4)
  , sequencer_data_port(0x3c5)
  , graphics_controller_index_port(0x3ce)
  , graphics_controller_data_port(0x3cf)
  , attribute_controller_index_port(0x3c0)
  , attribute_controller_read_port(0x3c1)
  , attribute_controller_write_port(0x3c0)
  , attribute_controller_reset_port(0x3da)
{}

VGA::~VGA() {}

void
VGA::write_registers(u8* registers)
{
  misc_port.write(*(registers++));

  for (u8 i = 0; i < 5; i++) {
    sequencer_index_port.write(i);
    sequencer_data_port.write(*(registers++));
  }

  crtc_index_port.write(0x03);
  crtc_data_port.write(crtc_data_port.read() | 0x80);
  crtc_index_port.write(0x11);
  crtc_data_port.write(crtc_data_port.read() & ~0x80);

  registers[0x03] = registers[0x03] | 0x80;
  registers[0x11] = registers[0x11] & ~0x80;

  for (u8 i = 0; i < 25; i++) {
    crtc_index_port.write(i);
    crtc_data_port.write(*(registers++));
  }

  for (u8 i = 0; i < 9; i++) {
    graphics_controller_index_port.write(i);
    graphics_controller_data_port.write(*(registers++));
  }

  for (u8 i = 0; i < 21; i++) {
    attribute_controller_reset_port.read();
    attribute_controller_index_port.write(i);
    attribute_controller_write_port.write(*(registers++));
  }

  attribute_controller_reset_port.read();
  attribute_controller_index_port.write(0x20);
}

bool
VGA::supports_mode(u32 width, u32 height, u32 colordepth)
{
  return width == 320 && height == 200 && colordepth == 8;
}

bool
VGA::set_mode(u32 width, u32 height, u32 colordepth)
{
  if (!supports_mode(width, height, colordepth)) {
    return false;
  }

  unsigned char g_320x200x256[] = { /* MISC */
                                    0x63,
                                    /* SEQ */
                                    0x03,
                                    0x01,
                                    0x0F,
                                    0x00,
                                    0x0E,
                                    /* CRTC */
                                    0x5F,
                                    0x4F,
                                    0x50,
                                    0x82,
                                    0x54,
                                    0x80,
                                    0xBF,
                                    0x1F,
                                    0x00,
                                    0x41,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x9C,
                                    0x0E,
                                    0x8F,
                                    0x28,
                                    0x40,
                                    0x96,
                                    0xB9,
                                    0xA3,
                                    0xFF,
                                    /* GC */
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x40,
                                    0x05,
                                    0x0F,
                                    0xFF,
                                    /* AC */
                                    0x00,
                                    0x01,
                                    0x02,
                                    0x03,
                                    0x04,
                                    0x05,
                                    0x06,
                                    0x07,
                                    0x08,
                                    0x09,
                                    0x0A,
                                    0x0B,
                                    0x0C,
                                    0x0D,
                                    0x0E,
                                    0x0F,
                                    0x41,
                                    0x00,
                                    0x0F,
                                    0x00,
                                    0x00
  };

  write_registers(g_320x200x256);
  return true;
}

u8*
VGA::get_frame_buffer_segment()
{
  graphics_controller_index_port.write(0x06);
  u8 segment_number = graphics_controller_data_port.read() & (3 << 2);
  switch (segment_number) {
    default:
    case 0 << 2:
      return (u8*)0x00000;
    case 1 << 2:
      return (u8*)0xA0000;
    case 2 << 2:
      return (u8*)0xB0000;
    case 3 << 2:
      return (u8*)0xB8000;
  }
}

void
VGA::put_pixel(u32 x, u32 y, u8 color_index)
{
  u8* pixel_address = get_frame_buffer_segment() + 320 * y + x;
  *pixel_address = color_index;
}

u8
VGA::get_color_index(u8 r, u8 g, u8 b)
{
  if (r == 0x00, g == 0x00, b == 0xA8) {
    return 0x01;
  }
  return 0x00;
}

void
VGA::put_pixel(u32 x, u32 y, u8 r, u8 g, u8 b)
{
  put_pixel(x, y, get_color_index(r, g, b));
}
