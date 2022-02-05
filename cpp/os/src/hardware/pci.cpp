#include "hardware/pci.hpp"

PeripheralComponentInterconnectDeviceDescriptor::
  PeripheralComponentInterconnectDeviceDescriptor()
{}

PeripheralComponentInterconnectDeviceDescriptor::
  ~PeripheralComponentInterconnectDeviceDescriptor()
{}

PeripheralComponentInterconnectController::
  PeripheralComponentInterconnectController()
  : dataPort(0xCFC)
  , commandPort(0xCF8)
{}

PeripheralComponentInterconnectController::
  ~PeripheralComponentInterconnectController()
{}

u32
PeripheralComponentInterconnectController::read(u16 bus,
                                                u16 device,
                                                u16 fn,
                                                u32 register_offset)
{
  u32 id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) |
           ((fn & 0x07) << 8) | (register_offset & 0xFC);
  commandPort.write(id);
  u32 result = dataPort.read();
  return result >> (8 * (register_offset % 4));
}

void
PeripheralComponentInterconnectController::write(u16 bus,
                                                 u16 device,
                                                 u16 fn,
                                                 u32 register_offset,
                                                 u32 value)
{
  u32 id = 0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) |
           ((fn & 0x07) << 8) | (register_offset & 0xFC);
  commandPort.write(id);
  dataPort.write(value);
}

bool
PeripheralComponentInterconnectController::device_has_functions(u16 bus,
                                                                u16 device)
{
  return read(bus, device, 0, 0x0E) & (1 << 7);
}

void
PeripheralComponentInterconnectController::select_drivers(
  DriverManager* driver_manager,
  InterruptManager* interrupt_manager)
{
  for (int bus = 0; bus < 8; bus++) {
    for (int device = 0; device < 32; device++) {
      int total_fn = device_has_functions(bus, device) ? 8 : 1;
      for (int fn = 0; fn < total_fn; fn++) {
        PeripheralComponentInterconnectDeviceDescriptor dev =
          get_device_descriptor(bus, device, fn);

        if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF) {
          continue;
        }

        for (int total_bar = 0; total_bar < 6; total_bar++) {
          auto bar = get_base_address_register(bus, device, fn, total_bar);
          if (bar.address &&
              (bar.type == BaseAddressRegisterType::InputOutput)) {
            dev.portBase = (u32)bar.address;
          }

          Driver* driver = get_driver(dev, interrupt_manager);
          if (driver != 0) {
            driver_manager->add(driver);
          }
        }

        printf("PCI BUS ");
        printh(bus & 0xFF);

        printf(", DEVICE ");
        printh(device & 0xFF);

        printf(", FUNCTION ");
        printh(fn & 0xFF);

        printf(" = VENDOR ");
        printh((dev.vendor_id & 0xFF00) >> 8);
        printh(dev.vendor_id & 0xFF);

        printf(", DEVICE ");
        printh((dev.device_id & 0xFF00) >> 8);
        printh(dev.device_id & 0xFF);

        printf("\n");
      }
    }
  }
}

PeripheralComponentInterconnectDeviceDescriptor
PeripheralComponentInterconnectController::get_device_descriptor(u16 bus,
                                                                 u16 device,
                                                                 u16 fn)
{
  PeripheralComponentInterconnectDeviceDescriptor result;

  result.bus = bus;
  result.device = device;
  result.fn = fn;

  result.vendor_id = read(bus, device, fn, 0x00);
  result.device_id = read(bus, device, fn, 0x02);

  result.class_id = read(bus, device, fn, 0x0b);
  result.subclass_id = read(bus, device, fn, 0x0a);
  result.interface_id = read(bus, device, fn, 0x09);

  result.revision = read(bus, device, fn, 0x08);
  result.interrupt = read(bus, device, fn, 0x3c);

  return result;
}

BaseAddressRegister
PeripheralComponentInterconnectController::get_base_address_register(u16 bus,
                                                                     u16 device,
                                                                     u16 fn,
                                                                     u16 bar)
{
  BaseAddressRegister result;

  u32 headertype = read(bus, device, fn, 0x0E) & 0x7F;
  int max_BARs = 6 - (4 * headertype);
  if (bar >= max_BARs) {
    return result;
  }

  u32 bar_value = read(bus, device, fn, 0x10 + 4 * bar);
  result.type = (bar_value & 0x1) ? BaseAddressRegisterType::InputOutput
                                  : BaseAddressRegisterType::MemoryMapping;

  u32 temp;

  if (result.type == BaseAddressRegisterType::MemoryMapping) {
    switch ((bar_value >> 1) & 0x3) {
      case 0: // 32 Bit
      case 1: // 20 Bit
      case 2: // 64 Bit
        break;
    }
  } else {
    result.address = (u8*)(bar_value & ~0x3);
    result.prefetchable = false;
  }

  return result;
}

Driver*
PeripheralComponentInterconnectController::get_driver(
  PeripheralComponentInterconnectDeviceDescriptor dev,
  InterruptManager* interrupts)
{
  switch (dev.vendor_id) {
    case 0x1022: // AMD
      switch (dev.device_id) {
        case 0x2000:
          printf("AMD am79c973 ");
          break;
      }
      break;

    case 0x8086: // Intel
      break;
  }

  switch (dev.class_id) {
    case 0x03: // graphics
      switch (dev.subclass_id) {
        case 0x00: // VGA
          printf("VGA ");
          break;
      }
      break;
  }

  return 0;
}
