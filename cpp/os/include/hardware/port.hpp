#ifndef __PORT_HPP__
#define __PORT_HPP__

#include "types.hpp"

class Port
{
protected:
  u16 port_number;

protected:
  Port(const u16 port_number);
  ~Port();
};

class Port8Bit : public Port
{
public:
  Port8Bit(const u16 port_number);
  ~Port8Bit();

public:
  virtual void write(const u8 data);
  virtual u8 read();

protected:
  static inline u8 read_8(const u16 port)
  {
    u8 result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
  }

  static inline void write_8(const u16 port, const u8 data)
  {
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
  }
};

class Port8BitSlow : public Port8Bit
{
public:
  Port8BitSlow(const u16 port_number);
  ~Port8BitSlow();

public:
  virtual void write(const u8 data);

protected:
  static inline void write8_slow(const u16 port, const u8 data)
  {
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:"
                     :
                     : "a"(data), "Nd"(port));
  }
};

class Port16Bit : public Port
{
public:
  Port16Bit(const u16 port_number);
  ~Port16Bit();

public:
  virtual void write(const u16 data);
  virtual const u16 read();

protected:
  static inline u16 read_16(const u16 port)
  {
    u16 result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
  }

  static inline void write_16(const u16 port, const u16 data)
  {
    __asm__ volatile("outw %0, %1" : : "a"(data), "Nd"(port));
  }
};

class Port32Bit : public Port
{
public:
  Port32Bit(const u16 port_number);
  ~Port32Bit();

public:
  virtual void write(const u32 data);
  virtual u32 read();

protected:
  static inline u32 read_32(const u16 port)
  {
    u32 result;
    __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(port));
    return result;
  }

  static inline void write_32(const u16 port, const u32 data)
  {
    __asm__ volatile("outl %0, %1" : : "a"(data), "Nd"(port));
  }
};

#endif
