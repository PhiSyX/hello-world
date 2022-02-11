#include "net/etherframe.hpp"
#include "memory.hpp"

EtherFrameHandler::EtherFrameHandler(EtherFrameProvider* $backend,
                                     const u16 ether_type)
{
  ether_type_be = ((ether_type & 0x00FF) << 8) | ((ether_type & 0xFF00) >> 8);
  backend = $backend;
  $backend->handlers[ether_type_be] = this;
}

EtherFrameHandler::~EtherFrameHandler()
{
  if (backend->handlers[ether_type_be] == this) {
    backend->handlers[ether_type_be] = 0;
  }
}

const bool
EtherFrameHandler::on_etherframe_recv(const u8* etherframe_payload,
                                      const u32 size) const
{
  return false;
}

void
EtherFrameHandler::send(const u64 dst_mac_be, const u8* data, const u32 size)
{
  backend->send(dst_mac_be, ether_type_be, data, size);
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
EtherFrameProvider::on_rawdata_recv(const u8* buffer, const u32 size) const
{
  if (size < sizeof(EtherFrameHeader)) {
    return false;
  }

  EtherFrameHeader* frame = (EtherFrameHeader*)buffer;
  bool send_back = false;

  if (frame->dst_mac_be == 0xFFFFFFFFFFFF ||
      frame->dst_mac_be == backend->get_mac_address()) {
    if (handlers[frame->ether_type_be] != 0) {
      send_back = handlers[frame->ether_type_be]->on_etherframe_recv(
        buffer + sizeof(EtherFrameHeader), size - sizeof(EtherFrameHeader));
    }
  }

  if (send_back) {
    frame->dst_mac_be = frame->src_mac_be;
    frame->src_mac_be = backend->get_mac_address();
  }

  return send_back;
}

void
EtherFrameProvider::send(const u64 dst_mac_be,
                         const u16 ether_type_be,
                         const u8* buffer,
                         const u32 size)
{
  u8* buffer2 = (u8*)MemoryManager::active_memory_manager->malloc(
    sizeof(EtherFrameHeader) + size);
  EtherFrameHeader* frame = (EtherFrameHeader*)buffer2;

  frame->dst_mac_be = dst_mac_be;
  frame->src_mac_be = backend->get_mac_address();
  frame->ether_type_be = ether_type_be;

  const u8* src = buffer;
  u8* dst = buffer2 + sizeof(EtherFrameHeader);
  for (u32 i = 0; i < size; i++) {
    dst[i] = src[i];
  }

  backend->send(buffer2, size + sizeof(EtherFrameHeader));

  MemoryManager::active_memory_manager->free(buffer2);
}

const u32
EtherFrameProvider::get_ip_address() const
{
  return backend->get_ip_address();
}

const u64
EtherFrameProvider::get_mac_address() const
{
  return backend->get_mac_address();
}

const u32
EtherFrameHandler::get_ip_address() const
{
  return backend->get_ip_address();
}
