#ifndef __PORT_HPP__
#define __PORT_HPP__

#include "types.hpp"

class Port
{
protected:
  u16 portnumber;

protected:
  Port(u16 portnumber);
  ~Port();
};

class Port8Bit : public Port
{
public:
  Port8Bit(u16 portnumber);
  ~Port8Bit();

public:
  virtual void write(u8 data);
  virtual u8 read();

protected:
  static inline u8 read8(u16 _port)
  {
    u8 result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(_port));
    return result;
  }

  static inline void write8(u16 _port, u8 _data)
  {
    __asm__ volatile("outb %0, %1" : : "a"(_data), "Nd"(_port));
  }
};

class Port8BitSlow : public Port8Bit
{
public:
  Port8BitSlow(u16 portnumber);
  ~Port8BitSlow();

public:
  virtual void write(u8 data);

protected:
  static inline void write8_slow(u16 _port, u8 _data)
  {
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:"
                     :
                     : "a"(_data), "Nd"(_port));
  }
};

class Port16Bit : public Port
{
public:
  Port16Bit(u16 portnumber);
  ~Port16Bit();

public:
  virtual void write(u16 data);
  virtual u16 read();

protected:
  static inline u16 read16(u16 _port)
  {
    u16 result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(_port));
    return result;
  }

  static inline void write16(u16 _port, u16 _data)
  {
    __asm__ volatile("outw %0, %1" : : "a"(_data), "Nd"(_port));
  }
};

class Port32Bit : public Port
{
public:
  Port32Bit(u16 portnumber);
  ~Port32Bit();

public:
  virtual void write(u32 data);
  virtual u32 read();

protected:
  static inline u32 read32(u16 _port)
  {
    u32 result;
    __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(_port));
    return result;
  }

  static inline void write32(u16 _port, u32 _data)
  {
    __asm__ volatile("outl %0, %1" : : "a"(_data), "Nd"(_port));
  }
};

#endif
