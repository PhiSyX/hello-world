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
  const bool on_ip_recv(const u32 src_ip_be,
                        const u32 dst_ip_be,
                        const u8* ip_payload,
                        const u32 size) const;
  void request_echo_reply(const u32 ip_be);
};

#endif
