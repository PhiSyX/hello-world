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
  ATA(bool master, u16 port_base);
  ~ATA();

public:
  void identify();
  void read28(u32 sector_num, i32 count = 512);
  void write28(u32 sector_num, u8* data, u32 count);
  void flush();
};

#endif
