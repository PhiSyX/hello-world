#ifndef __PCI_HPP__
#define __PCI_HPP__

#include "drivers/driver.hpp"
#include "hardware/interrupts.hpp"
#include "hardware/port.hpp"
#include "types.hpp"

void
printf(char* str);
void printh(u8);

class PeripheralComponentInterconnectDeviceDescriptor
{
public:
  u32 portBase;
  u32 interrupt;

  u16 bus;
  u16 device;
  u16 fn;

  u16 vendor_id;
  u16 device_id;

  u8 class_id;
  u8 subclass_id;
  u8 interface_id;

  u8 revision;

public:
  PeripheralComponentInterconnectDeviceDescriptor();
  ~PeripheralComponentInterconnectDeviceDescriptor();
};

class PeripheralComponentInterconnectController
{
  Port32Bit dataPort;
  Port32Bit commandPort;

public:
  PeripheralComponentInterconnectController();
  ~PeripheralComponentInterconnectController();

public:
  u32 read(u16 bus, u16 device, u16 fn, u32 register_offset);
  void write(u16 bus, u16 device, u16 fn, u32 register_offset, u32 value);
  bool device_has_functions(u16 bus, u16 device);
  void select_drivers(DriverManager* driver_manager);
  PeripheralComponentInterconnectDeviceDescriptor
  get_device_descriptor(u16 bus, u16 device, u16 fn);
};

#endif
