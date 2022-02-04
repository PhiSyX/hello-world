#include "interrupts.hpp"

void
printf(char* str);

InterruptManager::GateDescriptor
  InterruptManager::interrupt_descriptor_table[256];

void
InterruptManager::set_interrupt_descriptor(u8 interrupt_number,
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
  printf(" INTERRUPT");

  return esp;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
  : pic_master_command(0x20)
  , pic_master_data(0x21)
  , pic_slave_command(0xA0)
  , pic_slave_data(0xA1)
{
  const u8 IDT_INTERRUPT_GATE = 0xE;

  u16 code_segment = gdt->CodeSegmentSelector();

  for (u16 i = 0; i < 256; i++) {
    set_interrupt_descriptor(
      i, code_segment, &ignore_interrupt_request, 0, IDT_INTERRUPT_GATE);
  }

  set_interrupt_descriptor(
    0x20, code_segment, &handle_interrupt_request0x00, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor(
    0x21, code_segment, &handle_interrupt_request0x01, 0, IDT_INTERRUPT_GATE);

  pic_master_command.write(0x11);
  pic_slave_command.write(0x11);

  pic_master_data.write(0x20);
  pic_slave_data.write(0x20);
  pic_master_data.write(0x04);
  pic_slave_data.write(0x02);
  pic_master_data.write(0x01);
  pic_slave_data.write(0x01);
  pic_master_data.write(0x00);
  pic_slave_data.write(0x00);

  InterruptDescriptorTablePtr idt;
  idt.size = 256 * sizeof(GateDescriptor) - 1;
  idt.base = (u32)interrupt_descriptor_table;

  asm volatile("lidt %0" : : "m"(idt));
}

InterruptManager::~InterruptManager() {}

void
InterruptManager::activate()
{
  asm("sti");
}
