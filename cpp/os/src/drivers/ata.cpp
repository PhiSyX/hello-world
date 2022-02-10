#include "drivers/ata.hpp"

ATA::ATA(bool $master, u16 port_base)
  : data_port(port_base)
  , error_port(port_base + 0x1)
  , sector_count_port(port_base + 0x2)
  , lba_low_port(port_base + 0x3)
  , lba_mid_port(port_base + 0x4)
  , lba_hi_port(port_base + 0x5)
  , device_port(port_base + 0x6)
  , command_port(port_base + 0x7)
  , control_port(port_base + 0x206)
{
  master = $master;
}

ATA::~ATA() {}

void
ATA::identify()
{
  device_port.write(master ? 0xA0 : 0xB0);
  control_port.write(0);

  device_port.write(0xA0);
  u8 status = command_port.read();
  if (status == 0xFF)
    return;

  device_port.write(master ? 0xA0 : 0xB0);
  sector_count_port.write(0);
  lba_low_port.write(0);
  lba_mid_port.write(0);
  lba_hi_port.write(0);
  command_port.write(0xEC);

  status = command_port.read();
  if (status == 0x00) {
    return;
  }

  while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
    status = command_port.read();
  }

  if (status & 0x01) {
    printf("ERROR");
    return;
  }

  for (i32 i = 0; i < 256; i++) {
    u16 data = data_port.read();
    char* text = "  \0";
    text[0] = (data >> 8) & 0xFF;
    text[1] = data & 0xFF;
    printf(text);
  }

  printf("\n");
}

void
ATA::read28(u32 sectorNum, i32 count)
{
  if (sectorNum > 0x0FFFFFFF) {
    return;
  }

  device_port.write((master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24));
  error_port.write(0);
  sector_count_port.write(1);
  lba_low_port.write(sectorNum & 0x000000FF);
  lba_mid_port.write((sectorNum & 0x0000FF00) >> 8);
  lba_low_port.write((sectorNum & 0x00FF0000) >> 16);
  command_port.write(0x20);

  u8 status = command_port.read();
  while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
    status = command_port.read();
  }

  if (status & 0x01) {
    printf("ERROR");
    return;
  }

  printf("Reading ATA Drive: ");

  for (i32 i = 0; i < count; i += 2) {
    u16 wdata = data_port.read();

    char* text = "  \0";
    text[0] = wdata & 0xFF;

    if (i + 1 < count) {
      text[1] = (wdata >> 8) & 0xFF;
    } else {
      text[1] = '\0';
    }

    printf(text);
  }

  for (i32 i = count + (count % 2); i < 512; i += 2) {
    data_port.read();
  }
}

void
ATA::write28(u32 sectorNum, u8* data, u32 count)
{
  if (sectorNum > 0x0FFFFFFF) {
    return;
  }
  if (count > 512) {
    return;
  }

  device_port.write((master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24));
  error_port.write(0);
  sector_count_port.write(1);
  lba_low_port.write(sectorNum & 0x000000FF);
  lba_mid_port.write((sectorNum & 0x0000FF00) >> 8);
  lba_low_port.write((sectorNum & 0x00FF0000) >> 16);
  command_port.write(0x30);

  printf("Writing to ATA Drive: ");

  for (i32 i = 0; i < count; i += 2) {
    u16 wdata = data[i];
    if (i + 1 < count) {
      wdata |= ((u16)data[i + 1]) << 8;
    }
    data_port.write(wdata);

    char* text = "  \0";
    text[0] = (wdata >> 8) & 0xFF;
    text[1] = wdata & 0xFF;
    printf(text);
  }

  for (i32 i = count + (count % 2); i < 512; i += 2) {
    data_port.write(0x0000);
  }
}

void
ATA::flush()
{
  device_port.write(master ? 0xE0 : 0xF0);
  command_port.write(0xE7);

  u8 status = command_port.read();
  if (status == 0x00) {
    return;
  }

  while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
    status = command_port.read();
  }

  if (status & 0x01) {
    printf("ERROR");
    return;
  }
}
