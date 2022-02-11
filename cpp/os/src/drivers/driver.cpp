#include "drivers/driver.hpp"

Driver::Driver() {}

Driver::~Driver() {}

void
Driver::activate() const
{}

const int
Driver::reset() const
{
  return 0;
}

void
Driver::deactivate() const
{}

DriverManager::DriverManager()
{
  total_drivers = 0;
}

const void
DriverManager::add(const Driver* driver) const
{
  drivers[total_drivers] = driver;
  total_drivers++;
}

const void
DriverManager::enable_all() const
{
  for (usize i = 0; i < total_drivers; i++) {
    const auto driver = drivers[i];
    driver->activate();
  }
}
