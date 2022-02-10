#ifndef __VGA_HPP__
#define __VGA_HPP__

#include "drivers/driver.hpp"
#include "hardware/port.hpp"
#include "types.hpp"

// Video Graphics Array
class VGA
{
protected:
  Port8Bit misc_port;
  Port8Bit crtc_index_port;
  Port8Bit crtc_data_port;
  Port8Bit sequencer_index_port;
  Port8Bit sequencer_data_port;
  Port8Bit graphics_controller_index_port;
  Port8Bit graphics_controller_data_port;
  Port8Bit attribute_controller_index_port;
  Port8Bit attribute_controller_read_port;
  Port8Bit attribute_controller_write_port;
  Port8Bit attribute_controller_reset_port;

  u8* get_frame_buffer_segment();
  virtual u8 get_color_index(u8 r, u8 g, u8 b);
  void write_registers(u8* registers);

public:
  VGA();
  ~VGA();

  virtual const bool supports_mode(u32 width, u32 height, u32 colordepth) const;
  virtual bool set_mode(u32 width, u32 height, u32 colordepth);
  virtual void put_pixel(i32 x, i32 y, u8 r, u8 g, u8 b);
  virtual void put_pixel(i32 x, i32 y, u8 color_index);
  virtual void fill_rect(u32 x, u32 y, u32 width, u32 height, u8 r, u8 g, u8 b);
};

#endif
