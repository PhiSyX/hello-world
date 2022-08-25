#ifndef __ATA_HPP__
#define __ATA_HPP__

#include "hardware/interrupts.hpp"
#include "hardware/port.hpp"
#include "types.hpp"

void
printf(char* str);
void printh(u8);

// Advanced Technology Attachment
class ATA
{
protected:
  bool master;

  Port16Bit data_port;
  Port8Bit error_port;
  Port8Bit sector_count_port;
  Port8Bit lba_low_port;
  Port8Bit lba_mid_port;
  Port8Bit lba_hi_port;
  Port8Bit device_port;
  Port8Bit command_port;
  Port8Bit control_port;

public:
  ATA(const bool master, const u16 port_base);
  ~ATA();

public:
  /* contains static */ void identify();
  /* contains static */ void read_28(const u32 sector_num,
                                     const i32 count = 512);
  /* contains static */ void write_28(const u32 sector_num,
                                      const u8* data,
                                      const u32 count);
  /* contains static */ void flush();
};

#endif
