#ifndef __AMD_AM79C973_HPP__
#define __AMD_AM79C973_HPP__

#include "drivers/driver.hpp"
#include "hardware/interrupts.hpp"
#include "hardware/pci.hpp"
#include "hardware/port.hpp"
#include "types.hpp"

void
printf(char*);
void printh(u8);

class amd_am79c973;

class RawDataHandler
{
protected:
  amd_am79c973* backend;

public:
  RawDataHandler(amd_am79c973* backend);
  ~RawDataHandler();

public:
  bool on_rawdata_recv(u8* buffer, u32 size);
  void send(u8* buffer, u32 size);
};

class amd_am79c973
  : public Driver
  , public InterruptHandler
{
  struct InitializationBlock
  {
    u16 mode;
    unsigned reserved1 : 4;
    unsigned total_send_buffers : 4;
    unsigned reserved2 : 4;
    unsigned total_recv_buffers : 4;
    u64 physical_address : 48;
    u16 reserved3;
    u64 logical_address;
    u32 recv_buffer_descriptor_address;
    u32 send_buffer_descriptor_address;
  } __attribute__((packed));

  struct BufferDescriptor
  {
    u32 address;
    u32 flags;
    u32 flags2;
    u32 avail;
  } __attribute__((packed));

  Port16Bit MAC_address0_port;
  Port16Bit MAC_address2_port;
  Port16Bit MAC_address4_port;
  Port16Bit register_data_port;
  Port16Bit register_address_port;
  Port16Bit reset_port;
  Port16Bit bus_control_register_data_port;

  InitializationBlock init_block;

  BufferDescriptor* send_buffer_descriptor;
  u8 send_buffer_descriptor_memory[2048 + 15];
  u8 send_buffers[2 * 1024 + 15][8];
  u8 current_send_buffer;

  BufferDescriptor* recv_buffer_descriptor;
  u8 recv_buffer_descriptor_memory[2048 + 15];
  u8 recv_buffers[2 * 1024 + 15][8];
  u8 current_recv_buffer;

  RawDataHandler* handler;

public:
  amd_am79c973(PCIDeviceDescriptor* device,
               InterruptManager* interrupt_manager);
  ~amd_am79c973();

public:
  void activate();
  i32 reset();

  u32 handle_interrupt(u32 esp);
  void send(u8* buffer, usize count);
  void recv();

  void set_handler(RawDataHandler* handler);
  u64 get_MAC_address();
  void set_IP_address(u32);
  u32 get_IP_address();
};

#endif
