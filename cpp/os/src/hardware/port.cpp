#include "hardware/port.hpp"

Port::Port(u16 $port_number)
{
  port_number = $port_number;
}

Port::~Port() {}

Port8Bit::Port8Bit(u16 port_number)
  : Port(port_number)
{}

Port8Bit::~Port8Bit() {}

void
Port8Bit::write(u8 data)
{
  write8(port_number, data);
}

u8
Port8Bit::read()
{
  return read8(port_number);
}

Port8BitSlow::Port8BitSlow(u16 port_number)
  : Port8Bit(port_number)
{}

Port8BitSlow::~Port8BitSlow() {}

void
Port8BitSlow::write(u8 data)
{
  write8_slow(port_number, data);
}

Port16Bit::Port16Bit(u16 port_number)
  : Port(port_number)
{}

Port16Bit::~Port16Bit() {}

void
Port16Bit::write(u16 data)
{
  write16(port_number, data);
}

u16
Port16Bit::read()
{
  return read16(port_number);
}

Port32Bit::Port32Bit(u16 port_number)
  : Port(port_number)
{}

Port32Bit::~Port32Bit() {}

void
Port32Bit::write(u32 data)
{
  write32(port_number, data);
}

u32
Port32Bit::read()
{
  return read32(port_number);
}
