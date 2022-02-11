#ifndef __ICMP_HPP__
#define __ICMP_HPP__

#include "net/ipv4.hpp"
#include "types.hpp"

void
printf(char*);
void printh(u8);

// Internet Control Message Protocol
struct ICMPMessage
{
  u8 type;
  u8 code;
  u16 checksum;
  u32 data;
} __attribute__((packed));

class ICMP : IPHandler
{
public:
  ICMP(IPProvider* backend);
  ~ICMP();

public:
  const bool on_ip_recv(u32 src_IP_BE,
                        u32 dst_IP_BE,
                        u8* ip_payload,
                        u32 size) const;
  void request_echo_reply(u32 IP_BE);
};

#endif
