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

  u32 src_ip;
  u32 dst_ip;
} __attribute__((packed));

class IPProvider;

class IPHandler
{
protected:
  IPProvider* backend;
  u8 ip_protocol;

public:
  IPHandler(IPProvider* backend, const u8 protocol);
  ~IPHandler();

  virtual const bool on_ip_recv(const u32 src_ip_be,
                                const u32 dst_ip_be,
                                const u8* ip_payload,
                                const u32 size) const;
  void send(const u32 dst_ip_be, const u8* ip_payload, const u32 size);
};

class IPProvider : public EtherFrameHandler
{
  friend class IPHandler;

protected:
  IPHandler* handlers[255];
  ARP* arp;
  u32 gateway_ip;
  u32 subnet_mask;

public:
  IPProvider(EtherFrameProvider* backend,
             ARP* arp,
             const u32 gateway_ip,
             const u32 subnet_mask);
  ~IPProvider();

public:
  const bool on_etherframe_recv(const u8* etherframe_payload,
                                const u32 size) const;
  void send(const u32 dst_ip_be,
            const u8 protocol,
            const u8* buffer,
            const u32 size);
  static u16 checksum(const u16* data, const u32 length_in_bytes);
};

#endif
