#ifndef __PCI_HPP__
#define __PCI_HPP__

#include "drivers/driver.hpp"
#include "hardware/interrupts.hpp"
#include "hardware/port.hpp"
#include "types.hpp"

void
printf(char* str);
void printh(u8);

enum BaseAddressRegisterType
{
  MemoryMapping = 0,
  InputOutput = 1
};

class BaseAddressRegister
{
public:
  bool prefetchable;
  u8* address;
  u32 size;
  BaseAddressRegisterType type;
};

class PCIDeviceDescriptor
{
public:
  u32 port_base;
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
  PCIDeviceDescriptor();
  ~PCIDeviceDescriptor();
};

class PCIController
{
  Port32Bit data_port;
  Port32Bit command_port;

public:
  PCIController();
  ~PCIController();

public:
  u32 read(const u16 bus, const u16 dev, const u16 fn, const u32 ro);
  void write(const u16 bus,
             const u16 dev,
             const u16 fn,
             const u32 ro,
             const u32 value);
  bool device_has_functions(const u16 bus, const u16 dev);
  void select_drivers(const DriverManager* dm, InterruptManager* im);
  const PCIDeviceDescriptor get_device_descriptor(const u16 bus,
                                                  const u16 dev,
                                                  const u16 fn);

  const Driver* get_driver(const PCIDeviceDescriptor* dev,
                           InterruptManager* im) const;

  BaseAddressRegister get_base_address_register(const u16 bus,
                                                const u16 dev,
                                                const u16 fn,
                                                const u16 bar);
};

#endif
