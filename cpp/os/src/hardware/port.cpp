#include "hardware/port.hpp"

Port::Port(u16 portnumber)
{
  this->portnumber = portnumber;
}

Port::~Port() {}

Port8Bit::Port8Bit(u16 portnumber)
  : Port(portnumber)
{}

Port8Bit::~Port8Bit() {}

void
Port8Bit::write(u8 data)
{
  write8(portnumber, data);
}

u8
Port8Bit::read()
{
  return read8(portnumber);
}

Port8BitSlow::Port8BitSlow(u16 portnumber)
  : Port8Bit(portnumber)
{}

Port8BitSlow::~Port8BitSlow() {}

void
Port8BitSlow::write(u8 data)
{
  write8_slow(portnumber, data);
}

Port16Bit::Port16Bit(u16 portnumber)
  : Port(portnumber)
{}

Port16Bit::~Port16Bit() {}

void
Port16Bit::write(u16 data)
{
  write16(portnumber, data);
}

u16
Port16Bit::read()
{
  return read16(portnumber);
}

Port32Bit::Port32Bit(u16 portnumber)
  : Port(portnumber)
{}

Port32Bit::~Port32Bit() {}

void
Port32Bit::write(u32 data)
{
  write32(portnumber, data);
}

u32
Port32Bit::read()
{
  return read32(portnumber);
}
