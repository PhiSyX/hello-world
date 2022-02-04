#include "interrupts.hpp"

void
printf(char* str);

InterruptManager::GateDescriptor
  InterruptManager::interrupt_descriptor_table[256];

void
InterruptManager::set_interrupt_descriptor_table_entry(
  u8 interrupt_number,
  u16 code_segment_selector_offset,
  void (*handler)(),
  u8 descriptor_privilege_level,
  u8 descriptor_type)
{
  const u8 IDT_DESC_PRESENT = 0x80;

  interrupt_descriptor_table[interrupt_number].handler_address_low_bits =
    ((u32)handler) & 0xFFFF;
  interrupt_descriptor_table[interrupt_number].gdt_code_segment_selector =
    code_segment_selector_offset;
  interrupt_descriptor_table[interrupt_number].access =
    IDT_DESC_PRESENT | descriptor_type |
    ((descriptor_privilege_level & 3) << 5);
  interrupt_descriptor_table[interrupt_number].reserved = 0;
  interrupt_descriptor_table[interrupt_number].handler_address_high_bits =
    (((u32)handler) >> 16) & 0xFFFF;
}

u32
InterruptManager::handle_interrupt(u32 interrupt_number, u32 esp)
{
  char* foo = "INTERRUPT 0x00";
  char* hex = "0123456789ABCDEF";

  foo[12] = hex[(interrupt_number >> 4) & 0xF];
  foo[13] = hex[interrupt_number & 0xF];
  printf(foo);

  return esp;
}

InterruptManager::InterruptManager(u16 hw_interrupt_offset,
                                   GlobalDescriptorTable* gdt)
  : pic_master_command(0x20)
  , pic_master_data(0x21)
  , pic_slave_command(0xA0)
  , pic_slave_data(0xA1)
{
  this->hardware_interrupt_offset = hw_interrupt_offset;
  u16 code_segment = gdt->CodeSegmentSelector();

  const u8 IDT_INTERRUPT_GATE = 0xE;
  for (u8 i = 255; i > 0; --i) {
    set_interrupt_descriptor_table_entry(
      i, code_segment, &ignore_interrupt_request, 0, IDT_INTERRUPT_GATE);
  }

  set_interrupt_descriptor_table_entry(
    0, code_segment, &ignore_interrupt_request, 0, IDT_INTERRUPT_GATE);

  set_interrupt_descriptor_table_entry(
    0x00, code_segment, &handle_interrupt_exception0x00, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x01, code_segment, &handle_interrupt_exception0x01, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x02, code_segment, &handle_interrupt_exception0x02, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x03, code_segment, &handle_interrupt_exception0x03, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x04, code_segment, &handle_interrupt_exception0x04, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x05, code_segment, &handle_interrupt_exception0x05, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x06, code_segment, &handle_interrupt_exception0x06, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x07, code_segment, &handle_interrupt_exception0x07, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x08, code_segment, &handle_interrupt_exception0x08, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x09, code_segment, &handle_interrupt_exception0x09, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0A, code_segment, &handle_interrupt_exception0x0A, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0B, code_segment, &handle_interrupt_exception0x0B, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0C, code_segment, &handle_interrupt_exception0x0C, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0D, code_segment, &handle_interrupt_exception0x0D, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0E, code_segment, &handle_interrupt_exception0x0E, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0F, code_segment, &handle_interrupt_exception0x0F, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x10, code_segment, &handle_interrupt_exception0x10, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x11, code_segment, &handle_interrupt_exception0x11, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x12, code_segment, &handle_interrupt_exception0x12, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x13, code_segment, &handle_interrupt_exception0x13, 0, IDT_INTERRUPT_GATE);

  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x00,
                                       code_segment,
                                       &handle_interrupt_request0x00,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x01,
                                       code_segment,
                                       &handle_interrupt_request0x01,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x02,
                                       code_segment,
                                       &handle_interrupt_request0x02,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x03,
                                       code_segment,
                                       &handle_interrupt_request0x03,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x04,
                                       code_segment,
                                       &handle_interrupt_request0x04,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x05,
                                       code_segment,
                                       &handle_interrupt_request0x05,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x06,
                                       code_segment,
                                       &handle_interrupt_request0x06,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x07,
                                       code_segment,
                                       &handle_interrupt_request0x07,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x08,
                                       code_segment,
                                       &handle_interrupt_request0x08,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x09,
                                       code_segment,
                                       &handle_interrupt_request0x09,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x0A,
                                       code_segment,
                                       &handle_interrupt_request0x0A,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x0B,
                                       code_segment,
                                       &handle_interrupt_request0x0B,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x0C,
                                       code_segment,
                                       &handle_interrupt_request0x0C,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x0D,
                                       code_segment,
                                       &handle_interrupt_request0x0D,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x0E,
                                       code_segment,
                                       &handle_interrupt_request0x0E,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hardware_interrupt_offset + 0x0F,
                                       code_segment,
                                       &handle_interrupt_request0x0F,
                                       0,
                                       IDT_INTERRUPT_GATE);

  pic_master_command.write(0x11);
  pic_slave_command.write(0x11);

  pic_master_data.write(hardware_interrupt_offset);
  pic_slave_data.write(hardware_interrupt_offset + 8);

  pic_master_data.write(0x04);
  pic_slave_data.write(0x02);
  pic_master_data.write(0x01);
  pic_slave_data.write(0x01);
  pic_master_data.write(0x00);
  pic_slave_data.write(0x00);

  InterruptDescriptorTablePtr idt_ptr;
  idt_ptr.size = 256 * sizeof(GateDescriptor) - 1;
  idt_ptr.base = (u32)interrupt_descriptor_table;

  asm volatile("lidt %0" : : "m"(idt_ptr));
}

InterruptManager::~InterruptManager()
{
  deactivate();
}

void
InterruptManager::activate()
{
  asm("sti");
}

void
InterruptManager::deactivate()
{}
