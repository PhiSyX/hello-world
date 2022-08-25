#include "hardware/port.hpp"

Port::Port(const u16 $port_number)
{
  port_number = $port_number;
}

Port::~Port() {}

Port8Bit::Port8Bit(const u16 port_number)
  : Port(port_number)
{}

Port8Bit::~Port8Bit() {}

void
Port8Bit::write(const u8 data)
{
  write_8(port_number, data);
}

u8
Port8Bit::read()
{
  return read_8(port_number);
}

Port8BitSlow::Port8BitSlow(const u16 port_number)
  : Port8Bit(port_number)
{}

Port8BitSlow::~Port8BitSlow() {}

void
Port8BitSlow::write(const u8 data)
{
  write8_slow(port_number, data);
}

Port16Bit::Port16Bit(const u16 port_number)
  : Port(port_number)
{}

Port16Bit::~Port16Bit() {}

void
Port16Bit::write(const u16 data)
{
  write_16(port_number, data);
}

const u16
Port16Bit::read()
{
  return read_16(port_number);
}

Port32Bit::Port32Bit(const u16 port_number)
  : Port(port_number)
{}

Port32Bit::~Port32Bit() {}

void
Port32Bit::write(const u32 data)
{
  write_32(port_number, data);
}

u32
Port32Bit::read()
{
  return read_32(port_number);
}
