#include "drivers/driver.hpp"

Driver::Driver() {}

Driver::~Driver() {}

void
Driver::activate() const
{}

int
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

void
DriverManager::add(const Driver* driver)
{
  drivers[total_drivers] = driver;
  total_drivers++;
}

void
DriverManager::enable_all() const
{
  for (usize i = 0; i < total_drivers; i++) {
    auto driver = drivers[i];
    driver->activate();
  }
}
