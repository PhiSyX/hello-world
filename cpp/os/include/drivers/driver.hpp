#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include "types.hpp"

class Driver
{
public:
  Driver();
  ~Driver();

public:
  virtual void activate();
  virtual i32 reset();
  virtual void deactivate();
};

class DriverManager
{
public:
  Driver* drivers[265];
  usize total_drivers;

public:
  DriverManager();

public:
  void add(Driver*);

  void enable_all();
};

#endif
