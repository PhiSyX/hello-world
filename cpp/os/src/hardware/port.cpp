#include "hardware/port.hpp"

Port::Port(u16 portnumber)
{
  this->portnumber = portnumber;
}

Port::~Port() {}

// u8

Port8Bit::Port8Bit(u16 portnumber)
  : Port(portnumber)
{}

Port8Bit::~Port8Bit() {}

void
Port8Bit::write(u8 data)
{
  __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(portnumber));
}

u8
Port8Bit::read()
{
  u8 result;
  __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(portnumber));
  return result;
}

// u8 Slow

Port8BitSlow::Port8BitSlow(u16 portnumber)
  : Port8Bit(portnumber)
{}

Port8BitSlow::~Port8BitSlow() {}

void
Port8BitSlow::write(u8 data)
{
  __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:"
                   :
                   : "a"(data), "Nd"(portnumber));
}

// u16

Port16Bit::Port16Bit(u16 portnumber)
  : Port(portnumber)
{}

Port16Bit::~Port16Bit() {}

void
Port16Bit::write(u16 data)
{
  __asm__ volatile("outw %0, %1" : : "a"(data), "Nd"(portnumber));
}

u16
Port16Bit::read()
{
  u16 result;
  __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(portnumber));
  return result;
}

// u32

Port32Bit::Port32Bit(u16 portnumber)
  : Port(portnumber)
{}

Port32Bit::~Port32Bit() {}

void
Port32Bit::write(u32 data)
{
  __asm__ volatile("outl %0, %1" : : "a"(data), "Nd"(portnumber));
}

u32
Port32Bit::read()
{
  u32 result;
  __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(portnumber));
  return result;
}
