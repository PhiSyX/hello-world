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
};

class Port8BitSlow : public Port8Bit
{
public:
  Port8BitSlow(u16 portnumber);
  ~Port8BitSlow();

public:
  virtual void write(u8 data);
};

class Port16Bit : public Port
{
public:
  Port16Bit(u16 portnumber);
  ~Port16Bit();

public:
  virtual void write(u16 data);
  virtual u16 read();
};

class Port32Bit : public Port
{
public:
  Port32Bit(u16 portnumber);
  ~Port32Bit();

public:
  virtual void write(u32 data);
  virtual u32 read();
};

#endif
