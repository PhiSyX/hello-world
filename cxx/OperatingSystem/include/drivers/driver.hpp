#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include "types.hpp"

class Driver
{
public:
  Driver();
  ~Driver();

public:
  virtual void activate() const;
  virtual const i32 reset() const;
  virtual void deactivate() const;
};

class DriverManager
{
public:
  mutable const Driver* drivers[265];
  mutable usize total_drivers;

public:
  DriverManager();

public:
  const void add(const Driver*) const;

  const void enable_all() const;
};

#endif
