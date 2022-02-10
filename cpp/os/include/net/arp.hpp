#ifndef __ARP_HPP__
#define __ARP_HPP__

#include "net/etherframe.hpp"
#include "types.hpp"

// Address Resolution Protocol Message
struct ARPMessage
{
  u16 hardware_type;
  u16 protocol;
  u8 hardware_address_size; // 6
  u8 protocol_address_size; // 4
  u16 command;

  u64 src_MAC : 48;
  u32 src_IP;
  u64 dst_MAC : 48;
  u32 dst_IP;

} __attribute__((packed));

// Address Resolution Protocol
class ARP : public EtherFrameHandler
{
  u32 IP_cache[128];
  u64 MAC_cache[128];
  int total_cache_entries;

public:
  ARP(EtherFrameProvider* backend);
  ~ARP();

public:
  bool on_etherframe_recv(u8* etherframe_payload, u32 size);

  void request_MAC_address(u32 IP_BE);
  u64 get_MAC_from_cache(u32 IP_BE);
  u64 resolve(u32 IP_BE);
};

#endif
