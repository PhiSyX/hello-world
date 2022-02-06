#include "drivers/driver.hpp"

Driver::Driver() {}

Driver::~Driver() {}

void
Driver::activate()
{}

int
Driver::reset()
{
  return 0;
}

void
Driver::deactivate()
{}

DriverManager::DriverManager()
{
  total_drivers = 0;
}

void
DriverManager::add(Driver* driver)
{
  drivers[total_drivers] = driver;
  total_drivers++;
}

void
DriverManager::enable_all()
{
  for (int i = 0; i < total_drivers; i++) {
    auto driver = drivers[i];
    driver->activate();
  }
}
