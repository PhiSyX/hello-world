#ifndef __IPV4_HPP__
#define __IPV4_HPP__

#include "net/arp.hpp"
#include "net/etherframe.hpp"
#include "types.hpp"

struct IPV4Message
{
  u8 header_length : 4;
  u8 version : 4;
  u8 tos;
  u16 total_length;

  u16 ident;
  u16 flags_and_offset;

  u8 time_to_live;
  u8 protocol;
  u16 checksum;

  u32 src_IP;
  u32 dst_IP;
} __attribute__((packed));

class IPProvider;

class IPHandler
{
protected:
  IPProvider* backend;
  u8 ip_protocol;

public:
  IPHandler(IPProvider* backend, u8 protocol);
  ~IPHandler();

  virtual const bool on_ip_recv(u32 src_IP_BE,
                                u32 dst_IP_BE,
                                u8* ip_payload,
                                u32 size) const;
  void send(u32 dst_IP_BE, u8* ip_payload, u32 size);
};

class IPProvider : public EtherFrameHandler
{
  friend class IPHandler;

protected:
  IPHandler* handlers[255];
  ARP* arp;
  u32 gateway_IP;
  u32 subnet_mask;

public:
  IPProvider(EtherFrameProvider* backend,
             ARP* arp,
             u32 gateway_IP,
             u32 subnet_mask);
  ~IPProvider();

public:
  const bool on_etherframe_recv(u8* etherframe_payload, u32 size) const;
  void send(u32 dst_IP_BE, u8 protocol, u8* buffer, u32 size);
  static u16 checksum(u16* data, u32 length_in_bytes);
};

#endif
