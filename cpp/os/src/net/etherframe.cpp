#include "net/etherframe.hpp"
#include "memory.hpp"

EtherFrameHandler::EtherFrameHandler(EtherFrameProvider* $backend,
                                     u16 ether_type)
{
  ether_type_BE = ((ether_type & 0x00FF) << 8) | ((ether_type & 0xFF00) >> 8);
  backend = $backend;
  $backend->handlers[ether_type_BE] = this;
}

EtherFrameHandler::~EtherFrameHandler()
{
  backend->handlers[ether_type_BE] = 0;
}

const bool
EtherFrameHandler::on_etherframe_recv(u8* etherframe_payload, u32 size) const
{
  return false;
}

void
EtherFrameHandler::send(u64 dst_MAC_BE, u8* data, u32 size)
{
  backend->send(dst_MAC_BE, ether_type_BE, data, size);
}

EtherFrameProvider::EtherFrameProvider(amd_am79c973* backend)
  : RawDataHandler(backend)
{
  for (u32 i = 0; i < 65535; i++) {
    handlers[i] = 0;
  }
}

EtherFrameProvider::~EtherFrameProvider() {}

const bool
EtherFrameProvider::on_rawdata_recv(u8* buffer, u32 size) const
{
  EtherFrameHeader* frame = (EtherFrameHeader*)buffer;
  bool sendBack = false;

  if (frame->dst_MAC_BE == 0xFFFFFFFFFFFF ||
      frame->dst_MAC_BE == backend->get_MAC_address()) {
    if (handlers[frame->ether_type_BE] != 0) {
      sendBack = handlers[frame->ether_type_BE]->on_etherframe_recv(
        buffer + sizeof(EtherFrameHeader), size - sizeof(EtherFrameHeader));
    }
  }

  if (sendBack) {
    frame->dst_MAC_BE = frame->src_MAC_BE;
    frame->src_MAC_BE = backend->get_MAC_address();
  }

  return sendBack;
}

void
EtherFrameProvider::send(u64 dst_MAC_BE,
                         u16 ether_type_BE,
                         u8* buffer,
                         u32 size)
{
  u8* buffer2 = (u8*)MemoryManager::active_memory_manager->malloc(
    sizeof(EtherFrameHeader) + size);
  EtherFrameHeader* frame = (EtherFrameHeader*)buffer2;

  frame->dst_MAC_BE = dst_MAC_BE;
  frame->src_MAC_BE = backend->get_MAC_address();
  frame->ether_type_BE = ether_type_BE;

  u8* src = buffer;
  u8* dst = buffer2 + sizeof(EtherFrameHeader);
  for (u32 i = 0; i < size; i++) {
    dst[i] = src[i];
  }

  backend->send(buffer2, size + sizeof(EtherFrameHeader));
}

const u32
EtherFrameProvider::get_IP_address() const
{
  return backend->get_IP_address();
}

const u64
EtherFrameProvider::get_MAC_address() const
{
  return backend->get_MAC_address();
}
