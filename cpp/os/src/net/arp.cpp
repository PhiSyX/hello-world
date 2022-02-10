
#include "net/arp.hpp"

ARP::ARP(EtherFrameProvider* backend)
  : EtherFrameHandler(backend, 0x806)
{
  total_cache_entries = 0;
}

ARP::~ARP() {}

bool
ARP::on_etherframe_recv(u8* etherframe_payload, u32 size)
{
  if (size < sizeof(ARPMessage)) {
    return false;
  }

  ARPMessage* arp = (ARPMessage*)etherframe_payload;
  if (arp->hardware_type == 0x0100) {
    if (arp->protocol == 0x0008 && arp->hardware_address_size == 6 &&
        arp->protocol_address_size == 4 &&
        arp->dst_IP == backend->get_IP_address()) {

      switch (arp->command) {
        case 0x0100: // request
          arp->command = 0x0200;
          arp->dst_IP = arp->src_IP;
          arp->dst_MAC = arp->src_MAC;
          arp->src_IP = backend->get_IP_address();
          arp->src_MAC = backend->get_MAC_address();
          return true;
          break;

        case 0x0200: // response
          if (total_cache_entries < 128) {
            IP_cache[total_cache_entries] = arp->src_IP;
            MAC_cache[total_cache_entries] = arp->src_MAC;
            total_cache_entries++;
          }
          break;
      }
    }
  }

  return false;
}

void
ARP::request_MAC_address(u32 IP_BE)
{
  ARPMessage arp;
  arp.hardware_type = 0x0100;    // ethernet
  arp.protocol = 0x0008;         // ipv4
  arp.hardware_address_size = 6; // mac
  arp.protocol_address_size = 4; // ipv4
  arp.command = 0x0100;          // request

  arp.src_MAC = backend->get_MAC_address();
  arp.src_IP = backend->get_IP_address();
  arp.dst_MAC = 0xFFFFFFFFFFFF;
  arp.dst_IP = IP_BE;

  send(arp.dst_MAC, (u8*)&arp, sizeof(ARPMessage));
}

u64
ARP::get_MAC_from_cache(u32 IP_BE)
{
  for (int i = 0; i < total_cache_entries; i++) {
    if (IP_cache[i] == IP_BE) {
      return MAC_cache[i];
    }
  }
  return 0xFFFFFFFFFFFF;
}

u64
ARP::resolve(u32 IP_BE)
{
  u64 result = get_MAC_from_cache(IP_BE);
  if (result == 0xFFFFFFFFFFFF) {
    request_MAC_address(IP_BE);
  }

  while (result == 0xFFFFFFFFFFFF) {
    result = get_MAC_from_cache(IP_BE);
  }

  return result;
}
