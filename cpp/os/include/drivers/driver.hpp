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
  virtual i32 reset() const;
  virtual void deactivate() const;
};

class DriverManager
{
public:
  const Driver* drivers[265];
  usize total_drivers;

public:
  DriverManager();

public:
  void add(const Driver*);

  void enable_all() const;
};

#endif
