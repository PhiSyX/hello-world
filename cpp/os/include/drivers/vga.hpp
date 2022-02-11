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
  virtual const u8 get_color_index(const u8 r, const u8 g, const u8 b) const;
  void write_registers(u8* registers);

public:
  VGA();
  ~VGA();

public:
  virtual const bool supports_mode(const u32 width,
                                   const u32 height,
                                   const u32 colordepth) const;
  virtual /* contains static */ bool set_mode(const u32 width,
                                              const u32 height,
                                              const u32 colordepth);
  virtual /* contains static */ void put_pixel(const i32 x,
                                               const i32 y,
                                               const u8 r,
                                               const u8 g,
                                               const u8 b);
  virtual /* contains static */ void put_pixel(const i32 x,
                                               const i32 y,
                                               const u8 color_index);
  virtual /* contains static */ void fill_rect(const u32 x,
                                               const u32 y,
                                               const u32 width,
                                               const u32 height,
                                               const u8 r,
                                               const u8 g,
                                               const u8 b);
};

#endif
