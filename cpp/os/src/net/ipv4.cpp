#include "net/ipv4.hpp"
#include "memory.hpp"

IPHandler::IPHandler(IPProvider* $backend, u8 $protocol)
{
  backend = $backend;
  ip_protocol = $protocol;
  $backend->handlers[$protocol] = this;
}

IPHandler::~IPHandler()
{
  if (backend->handlers[ip_protocol] == this) {
    backend->handlers[ip_protocol] = 0;
  }
}

const bool
IPHandler::on_ip_recv(u32 srcIP_BE,
                      u32 dstIP_BE,
                      u8* ip_payload,
                      u32 size) const
{
  return false;
}

void
IPHandler::send(u32 dstIP_BE, u8* ip_payload, u32 size)
{
  backend->send(dstIP_BE, ip_protocol, ip_payload, size);
}

IPProvider::IPProvider(EtherFrameProvider* backend,
                       ARP* $arp,
                       u32 $gateway_IP,
                       u32 $subnet_mask)
  : EtherFrameHandler(backend, 0x800)
{
  for (i32 i = 0; i < 255; i++) {
    handlers[i] = 0;
  }
  arp = $arp;
  gateway_IP = $gateway_IP;
  subnet_mask = $subnet_mask;
}

IPProvider::~IPProvider() {}

const bool
IPProvider::on_etherframe_recv(u8* etherframe_payload, u32 size) const
{
  if (size < sizeof(IPV4Message)) {
    return false;
  }

  IPV4Message* ip_message = (IPV4Message*)etherframe_payload;
  bool send_back = false;

  if (ip_message->dst_IP == backend->get_IP_address()) {
    i32 length = ip_message->total_length;
    if (length > size) {
      length = size;
    }

    if (handlers[ip_message->protocol] != 0) {
      send_back = handlers[ip_message->protocol]->on_ip_recv(
        ip_message->src_IP,
        ip_message->dst_IP,
        etherframe_payload + 4 * ip_message->header_length,
        length - 4 * ip_message->header_length);
    }
  }

  if (send_back) {
    u32 temp = ip_message->dst_IP;
    ip_message->dst_IP = ip_message->src_IP;
    ip_message->src_IP = temp;

    ip_message->time_to_live = 0x40;
    ip_message->checksum =
      checksum((u16*)ip_message, 4 * ip_message->header_length);
  }

  return send_back;
}

void
IPProvider::send(u32 dstIP_BE, u8 protocol, u8* data, u32 size)
{

  u8* buffer = (u8*)MemoryManager::active_memory_manager->malloc(
    sizeof(IPV4Message) + size);
  IPV4Message* message = (IPV4Message*)buffer;

  message->version = 4;
  message->header_length = sizeof(IPV4Message) / 4;
  message->tos = 0;
  message->total_length = size + sizeof(IPV4Message);
  message->total_length = ((message->total_length & 0xFF00) >> 8) |
                          ((message->total_length & 0x00FF) << 8);
  message->ident = 0x0100;
  message->flags_and_offset = 0x0040;
  message->time_to_live = 0x40;
  message->protocol = protocol;

  message->dst_IP = dstIP_BE;
  message->src_IP = backend->get_IP_address();

  message->checksum = 0;
  message->checksum = checksum((u16*)message, sizeof(IPV4Message));

  u8* databuffer = buffer + sizeof(IPV4Message);
  for (i32 i = 0; i < size; i++) {
    databuffer[i] = data[i];
  }

  u32 route = dstIP_BE;
  if ((dstIP_BE & subnet_mask) != (message->src_IP & subnet_mask)) {
    route = gateway_IP;
  }

  backend->send(arp->resolve(route),
                this->ether_type_BE,
                buffer,
                sizeof(IPV4Message) + size);

  MemoryManager::active_memory_manager->free(buffer);
}

u16
IPProvider::checksum(u16* data, u32 lengthInBytes)
{
  u32 temp = 0;

  for (i32 i = 0; i < lengthInBytes / 2; i++) {
    temp += ((data[i] & 0xFF00) >> 8) | ((data[i] & 0x00FF) << 8);
  }

  if (lengthInBytes % 2) {
    temp += ((u16)((char*)data)[lengthInBytes - 1]) << 8;
  }

  while (temp & 0xFFFF0000) {
    temp = (temp & 0xFFFF) + (temp >> 16);
  }

  return ((temp & 0xFF00) >> 8) | ((temp & 0x00FF) << 8);
}
