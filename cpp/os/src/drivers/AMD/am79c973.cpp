#include "drivers/AMD/am79c973.hpp"

RawDataHandler::RawDataHandler(amd_am79c973* $backend)
{
  backend = $backend;
  $backend->set_handler(this);
}

RawDataHandler::~RawDataHandler()
{
  backend->set_handler(0);
}

bool
RawDataHandler::on_rawdata_recv(u8* buffer, u32 size)
{
  return false;
}

void
RawDataHandler::send(u8* buffer, u32 size)
{
  backend->send(buffer, size);
}

amd_am79c973::amd_am79c973(PCIDeviceDescriptor* device,
                           InterruptManager* interrupt_manager)
  : Driver()
  , InterruptHandler(interrupt_manager,
                     device->interrupt +
                       interrupt_manager->hardware_interrupt_offset)
  , MAC_address0_port(device->port_base)
  , MAC_address2_port(device->port_base + 0x02)
  , MAC_address4_port(device->port_base + 0x04)
  , register_data_port(device->port_base + 0x10)
  , register_address_port(device->port_base + 0x12)
  , reset_port(device->port_base + 0x14)
  , bus_control_register_data_port(device->port_base + 0x16)
{
  handler = 0;
  current_send_buffer = 0;
  current_recv_buffer = 0;

  u64 MAC0 = MAC_address0_port.read() % 256;
  u64 MAC1 = MAC_address0_port.read() / 256;
  u64 MAC2 = MAC_address2_port.read() % 256;
  u64 MAC3 = MAC_address2_port.read() / 256;
  u64 MAC4 = MAC_address4_port.read() % 256;
  u64 MAC5 = MAC_address4_port.read() / 256;

  u64 MAC =
    MAC5 << 40 | MAC4 << 32 | MAC3 << 24 | MAC2 << 16 | MAC1 << 8 | MAC0;

  register_address_port.write(20);
  bus_control_register_data_port.write(0x102);

  register_address_port.write(0);
  register_data_port.write(0x04);

  init_block.mode = 0x0000;
  init_block.reserved1 = 0;
  init_block.total_send_buffers = 3;
  init_block.reserved2 = 0;
  init_block.total_recv_buffers = 3;
  init_block.physical_address = MAC;
  init_block.reserved3 = 0;
  init_block.logical_address = 0;

  send_buffer_descriptor =
    (BufferDescriptor*)((((u32)&send_buffer_descriptor_memory[0]) + 15) &
                        ~((u32)0xF));
  init_block.send_buffer_descriptor_address = (u32)send_buffer_descriptor;
  recv_buffer_descriptor =
    (BufferDescriptor*)((((u32)&recv_buffer_descriptor_memory[0]) + 15) &
                        ~((u32)0xF));
  init_block.recv_buffer_descriptor_address = (u32)recv_buffer_descriptor;

  for (u8 i = 0; i < 8; i++) {
    send_buffer_descriptor[i].address =
      (((u32)&send_buffers[i]) + 15) & ~(u32)0xF;
    send_buffer_descriptor[i].flags = 0x7FF | 0xF000;
    send_buffer_descriptor[i].flags2 = 0;
    send_buffer_descriptor[i].avail = 0;

    recv_buffer_descriptor[i].address =
      (((u32)&recv_buffers[i]) + 15) & ~(u32)0xF;
    recv_buffer_descriptor[i].flags = 0xF7FF | 0x80000000;
    recv_buffer_descriptor[i].flags2 = 0;
    send_buffer_descriptor[i].avail = 0;
  }

  register_address_port.write(1);
  register_data_port.write((u32)(&init_block) & 0xFFFF);
  register_address_port.write(2);
  register_data_port.write(((u32)(&init_block) >> 16) & 0xFFFF);
}

amd_am79c973::~amd_am79c973() {}

void
amd_am79c973::activate()
{
  register_address_port.write(0);
  register_data_port.write(0x41);

  register_address_port.write(4);
  u32 temp = register_data_port.read();
  register_address_port.write(4);
  register_data_port.write(temp | 0xC00);

  register_address_port.write(0);
  register_data_port.write(0x42);
}

int
amd_am79c973::reset()
{
  reset_port.read();
  reset_port.write(0);
  return 10;
}

u32
amd_am79c973::handle_interrupt(u32 esp)
{
  register_address_port.write(0);
  u32 temp = register_data_port.read();

  if ((temp & 0x8000) == 0x8000) {
    printf("AMD am79c973 ERROR\n");
  }
  if ((temp & 0x2000) == 0x2000) {
    printf("AMD am79c973 COLLISION ERROR\n");
  }
  if ((temp & 0x1000) == 0x1000) {
    printf("AMD am79c973 MISSED FRAME\n");
  }
  if ((temp & 0x0800) == 0x0800) {
    printf("AMD am79c973 MEMORY ERROR\n");
  }
  if ((temp & 0x0400) == 0x0400) {
    recv();
  }
  if ((temp & 0x0200) == 0x0200) {
    printf(" SENT");
  }

  register_address_port.write(0);
  register_data_port.write(temp);

  if ((temp & 0x0100) == 0x0100) {
    printf("AMD am79c973 INIT DONE\n");
  }

  return esp;
}

void
amd_am79c973::send(u8* buffer, usize size)
{
  i32 send_descriptor = current_send_buffer;
  current_send_buffer = (current_send_buffer + 1) % 8;

  if (size > 1518) {
    size = 1518;
  }

  for (u8 *src = buffer + size - 1,
          *dst =
            (u8*)(send_buffer_descriptor[send_descriptor].address + size - 1);
       src >= buffer;
       src--, dst--) {
    *dst = *src;
  }

  printf("\nSENDING: ");
  for (i32 i = 0; i < (size > 64 ? 64 : size); i++) {
    printh(buffer[i]);
    printf(" ");
  }

  send_buffer_descriptor[send_descriptor].avail = 0;
  send_buffer_descriptor[send_descriptor].flags2 = 0;
  send_buffer_descriptor[send_descriptor].flags =
    0x8300F000 | ((u16)((-size) & 0xFFF));
  register_address_port.write(0);
  register_data_port.write(0x48);
}

void
amd_am79c973::recv()
{
  printf("\nRECEIVED: ");

  for (; (recv_buffer_descriptor[current_recv_buffer].flags & 0x80000000) == 0;
       current_recv_buffer = (current_recv_buffer + 1) % 8) {
    if (!(recv_buffer_descriptor[current_recv_buffer].flags & 0x40000000) &&
        (recv_buffer_descriptor[current_recv_buffer].flags & 0x03000000) ==
          0x03000000) {
      u32 size = recv_buffer_descriptor[current_recv_buffer].flags & 0xFFF;
      if (size > 64) {
        size -= 4;
      }

      u8* buffer = (u8*)(recv_buffer_descriptor[current_recv_buffer].address);

      for (i32 i = 0; i < (size > 64 ? 64 : size); i++) {
        printh(buffer[i]);
        printf(" ");
      }

      if (handler != 0) {
        if (handler->on_rawdata_recv(buffer, size)) {
          send(buffer, size);
        }
      }
    }

    recv_buffer_descriptor[current_recv_buffer].flags2 = 0;
    recv_buffer_descriptor[current_recv_buffer].flags = 0x8000F7FF;
  }
}

void
amd_am79c973::set_handler(RawDataHandler* $handler)
{
  handler = $handler;
}

const u64
amd_am79c973::get_MAC_address() const
{
  return init_block.physical_address;
}

void
amd_am79c973::set_IP_address(u32 ip)
{
  init_block.logical_address = ip;
}

const u32
amd_am79c973::get_IP_address() const
{
  return init_block.logical_address;
}
