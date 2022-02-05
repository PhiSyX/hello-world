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
  DriverManager* driver_manager)
{
  for (int bus = 0; bus < 8; bus++) {
    for (int device = 0; device < 32; device++) {
      int total_fn = device_has_functions(bus, device) ? 8 : 1;
      for (int fn = 0; fn < total_fn; fn++) {
        PeripheralComponentInterconnectDeviceDescriptor dev =
          get_device_descriptor(bus, device, fn);

        if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF) {
          break;
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
