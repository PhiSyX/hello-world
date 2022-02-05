#ifndef __INTERRUPTS_HPP__
#define __INTERRUPTS_HPP__

#include "gdt.hpp"
#include "hardware/port.hpp"
#include "types.hpp"

void
printf(char* str);

void
printh(u8 key);

class InterruptManager;

class InterruptHandler
{
protected:
  u8 interrupt_number;
  InterruptManager* interrupt_manager;

  InterruptHandler(InterruptManager* interrupt_manager, u8 interrupt_number);
  ~InterruptHandler();

public:
  virtual u32 handle_interrupt(u32 esp);
};

class InterruptManager
{
  friend class InterruptHandler;

protected:
  static InterruptManager* active_interrupt_manager;
  InterruptHandler* handlers[256];

  struct GateDescriptor
  {
    u16 handler_address_low_bits;
    u16 gdt_code_segment_selector;
    u8 reserved;
    u8 access;
    u16 handler_address_high_bits;
  } __attribute__((packed));

  static GateDescriptor idt[256];

  struct InterruptDescriptorTablePtr
  {
    u16 size;
    u32 base;
  } __attribute__((packed));

  u16 hardware_interrupt_offset;

  static void set_interrupt_descriptor_table_entry(
    u8 interrupt_number,
    u16 code_segment_selector_offset,
    void (*handler)(),
    u8 descriptor_privilege_level,
    u8 descriptor_type);

  static void interrupt_ignore();

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

  static void handle_exception0x00();
  static void handle_exception0x01();
  static void handle_exception0x02();
  static void handle_exception0x03();
  static void handle_exception0x04();
  static void handle_exception0x05();
  static void handle_exception0x06();
  static void handle_exception0x07();
  static void handle_exception0x08();
  static void handle_exception0x09();
  static void handle_exception0x0A();
  static void handle_exception0x0B();
  static void handle_exception0x0C();
  static void handle_exception0x0D();
  static void handle_exception0x0E();
  static void handle_exception0x0F();
  static void handle_exception0x10();
  static void handle_exception0x11();
  static void handle_exception0x12();
  static void handle_exception0x13();

  static u32 handle_interrupt(u8 interrupt_number, u32 esp);
  u32 do_handle_interrupt(u8 interrupt_number, u32 esp);

  Port8BitSlow pic_master_command_port;
  Port8BitSlow pic_master_data_port;
  Port8BitSlow pic_slave_command_port;
  Port8BitSlow pic_slave_data_port;

public:
  InterruptManager(u16 hw_interrupt_offset, GlobalDescriptorTable* gdt);
  ~InterruptManager();

public:
  void activate();
  void deactivate();
};

#endif
