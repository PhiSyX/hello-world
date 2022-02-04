#ifndef __INTERRUPTS_HPP__
#define __INTERRUPTS_HPP__

#include "gdt.hpp"
#include "port.hpp"
#include "types.hpp"

class InterruptManager
{

protected:
  struct GateDescriptor
  {
    u16 handler_address_low_bits;
    u16 gdt_code_segment_selector;
    u8 reserved;
    u8 access;
    u16 handler_address_high_bits;
  } __attribute__((packed));

  struct InterruptDescriptorTablePtr
  {
    u16 size;
    u32 base;
  } __attribute__((packed));

  static GateDescriptor interrupt_descriptor_table[256];

  u16 hardware_interrupt_offset;

  static void set_interrupt_descriptor_table_entry(
    u8 interrupt_number,
    u16 code_segment_selector_offset,
    void (*handler)(),
    u8 descriptor_privilege_level,
    u8 descriptor_type);

  Port8BitSlow pic_master_command;
  Port8BitSlow pic_master_data;
  Port8BitSlow pic_slave_command;
  Port8BitSlow pic_slave_data;

public:
  static u32 handle_interrupt(u32 interrupt_number, u32 esp);

  static void ignore_interrupt_request();

  static void handle_interrupt_request0x00();
  static void handle_interrupt_request0x01();
  static void handle_interrupt_request0x02();
  static void handle_interrupt_request0x03();
  static void handle_interrupt_request0x04();
  static void handle_interrupt_request0x05();
  static void handle_interrupt_request0x06();
  static void handle_interrupt_request0x07();
  static void handle_interrupt_request0x08();
  static void handle_interrupt_request0x09();
  static void handle_interrupt_request0x0A();
  static void handle_interrupt_request0x0B();
  static void handle_interrupt_request0x0C();
  static void handle_interrupt_request0x0D();
  static void handle_interrupt_request0x0E();
  static void handle_interrupt_request0x0F();
  static void handle_interrupt_request0x31();

  static void handle_interrupt_exception0x00();
  static void handle_interrupt_exception0x01();
  static void handle_interrupt_exception0x02();
  static void handle_interrupt_exception0x03();
  static void handle_interrupt_exception0x04();
  static void handle_interrupt_exception0x05();
  static void handle_interrupt_exception0x06();
  static void handle_interrupt_exception0x07();
  static void handle_interrupt_exception0x08();
  static void handle_interrupt_exception0x09();
  static void handle_interrupt_exception0x0A();
  static void handle_interrupt_exception0x0B();
  static void handle_interrupt_exception0x0C();
  static void handle_interrupt_exception0x0D();
  static void handle_interrupt_exception0x0E();
  static void handle_interrupt_exception0x0F();
  static void handle_interrupt_exception0x10();
  static void handle_interrupt_exception0x11();
  static void handle_interrupt_exception0x12();
  static void handle_interrupt_exception0x13();

public:
  InterruptManager(u16 hw_interrupt_offset, GlobalDescriptorTable* gdt);
  ~InterruptManager();

public:
  void activate();
  void deactivate();
};

#endif
