#include "hardware/interrupts.hpp"

InterruptHandler::InterruptHandler(InterruptManager* im, u8 in)
{
  interrupt_number = in;
  interrupt_manager = im;

  im->handlers[in] = this;
}

InterruptHandler::~InterruptHandler()
{
  if (interrupt_manager->handlers[interrupt_number] == this) {
    interrupt_manager->handlers[interrupt_number] = 0;
  }
}

u32
InterruptHandler::handle_interrupt(u32 esp)
{
  return esp;
}

InterruptManager::GateDescriptor InterruptManager::idt[256];
InterruptManager* InterruptManager::active_interrupt_manager = 0;

void
InterruptManager::set_interrupt_descriptor_table_entry(u8 in,
                                                       u16 cs,
                                                       void (*handler)(),
                                                       u8 dpl,
                                                       u8 dt)
{
  idt[in].handler_address_low_bits = ((u32)handler) & 0xFFFF;
  idt[in].handler_address_high_bits = (((u32)handler) >> 16) & 0xFFFF;
  idt[in].gdt_code_segment_selector = cs;

  const u8 IDT_DESC_PRESENT = 0x80;
  idt[in].access = IDT_DESC_PRESENT | ((dpl & 3) << 5) | dt;
  idt[in].reserved = 0;
}

InterruptManager::InterruptManager(u16 hw_interrupt_offset,
                                   GlobalDescriptorTable* gdt,
                                   TaskManager* tm)
  : pic_master_command_port(0x20)
  , pic_master_data_port(0x21)
  , pic_slave_command_port(0xA0)
  , pic_slave_data_port(0xA1)
{
  task_manager = tm;
  hardware_interrupt_offset = hw_interrupt_offset;
  u32 code_segment = gdt->get_code_segment_selector();

  const u8 IDT_INTERRUPT_GATE = 0xE;

  for (u8 i = 255; i > 0; --i) {
    set_interrupt_descriptor_table_entry(
      i, code_segment, &interrupt_ignore, 0, IDT_INTERRUPT_GATE);
    handlers[i] = 0;
  }
  set_interrupt_descriptor_table_entry(
    0, code_segment, &interrupt_ignore, 0, IDT_INTERRUPT_GATE);
  handlers[0] = 0;

  set_interrupt_descriptor_table_entry(
    0x00, code_segment, &handle_exception0x00, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x01, code_segment, &handle_exception0x01, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x02, code_segment, &handle_exception0x02, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x03, code_segment, &handle_exception0x03, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x04, code_segment, &handle_exception0x04, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x05, code_segment, &handle_exception0x05, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x06, code_segment, &handle_exception0x06, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x07, code_segment, &handle_exception0x07, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x08, code_segment, &handle_exception0x08, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x09, code_segment, &handle_exception0x09, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0A, code_segment, &handle_exception0x0A, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0B, code_segment, &handle_exception0x0B, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0C, code_segment, &handle_exception0x0C, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0D, code_segment, &handle_exception0x0D, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0E, code_segment, &handle_exception0x0E, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x0F, code_segment, &handle_exception0x0F, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x10, code_segment, &handle_exception0x10, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x11, code_segment, &handle_exception0x11, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x12, code_segment, &handle_exception0x12, 0, IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x13, code_segment, &handle_exception0x13, 0, IDT_INTERRUPT_GATE);

  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x00,
                                       code_segment,
                                       &handle_interrupt_request0x00,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x01,
                                       code_segment,
                                       &handle_interrupt_request0x01,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x02,
                                       code_segment,
                                       &handle_interrupt_request0x02,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x03,
                                       code_segment,
                                       &handle_interrupt_request0x03,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x04,
                                       code_segment,
                                       &handle_interrupt_request0x04,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x05,
                                       code_segment,
                                       &handle_interrupt_request0x05,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x06,
                                       code_segment,
                                       &handle_interrupt_request0x06,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x07,
                                       code_segment,
                                       &handle_interrupt_request0x07,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x08,
                                       code_segment,
                                       &handle_interrupt_request0x08,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x09,
                                       code_segment,
                                       &handle_interrupt_request0x09,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x0A,
                                       code_segment,
                                       &handle_interrupt_request0x0A,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x0B,
                                       code_segment,
                                       &handle_interrupt_request0x0B,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x0C,
                                       code_segment,
                                       &handle_interrupt_request0x0C,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x0D,
                                       code_segment,
                                       &handle_interrupt_request0x0D,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x0E,
                                       code_segment,
                                       &handle_interrupt_request0x0E,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(hw_interrupt_offset + 0x0F,
                                       code_segment,
                                       &handle_interrupt_request0x0F,
                                       0,
                                       IDT_INTERRUPT_GATE);
  set_interrupt_descriptor_table_entry(
    0x80, code_segment, &handle_interrupt_request0x80, 0, IDT_INTERRUPT_GATE);

  pic_master_command_port.write(0x11);
  pic_slave_command_port.write(0x11);

  pic_master_data_port.write(hw_interrupt_offset);
  pic_slave_data_port.write(hw_interrupt_offset + 8);

  pic_master_data_port.write(0x04);
  pic_slave_data_port.write(0x02);

  pic_master_data_port.write(0x01);
  pic_slave_data_port.write(0x01);

  pic_master_data_port.write(0x00);
  pic_slave_data_port.write(0x00);

  InterruptDescriptorTablePtr idt_ptr;
  idt_ptr.size = 256 * sizeof(GateDescriptor) - 1;
  idt_ptr.base = (u32)idt;
  asm volatile("lidt %0" : : "m"(idt_ptr));
}

InterruptManager::~InterruptManager()
{
  deactivate();
}

void
InterruptManager::activate()
{
  if (active_interrupt_manager != 0)
    active_interrupt_manager->deactivate();

  active_interrupt_manager = this;
  asm("sti");
}

void
InterruptManager::deactivate()
{
  if (active_interrupt_manager == this) {
    active_interrupt_manager = 0;
    asm("cli");
  }
}

u32
InterruptManager::handle_interrupt(u8 interrupt_number, u32 esp)
{
  if (active_interrupt_manager != 0) {
    return active_interrupt_manager->do_handle_interrupt(interrupt_number, esp);
  }
  return esp;
}

u32
InterruptManager::do_handle_interrupt(u8 interrupt_number, u32 esp)
{
  if (handlers[interrupt_number] != 0) {
    esp = handlers[interrupt_number]->handle_interrupt(esp);
  } else if (interrupt_number != hardware_interrupt_offset) {
    printf("UNHANDLED INTERRUPT 0x");
    printh(interrupt_number);
  }

  if (interrupt_number == hardware_interrupt_offset) {
    esp = (u32)task_manager->schedule((CPUState*)esp);
  }

  if (hardware_interrupt_offset <= interrupt_number &&
      interrupt_number < hardware_interrupt_offset + 16) {
    pic_master_command_port.write(0x20);
    if (hardware_interrupt_offset + 8 <= interrupt_number) {
      pic_slave_command_port.write(0x20);
    }
  }

  return esp;
}
