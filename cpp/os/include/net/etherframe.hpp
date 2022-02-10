#ifndef __ETHERFRAME_HPP__
#define __ETHERFRAME_HPP__

#include "drivers/AMD/am79c973.hpp"
#include "types.hpp"

struct EtherFrameHeader
{
  u64 dst_MAC_BE : 48;
  u64 src_MAC_BE : 48;
  u16 ether_type_BE;
} __attribute__((packed));

typedef u32 EtherFrameFooter;

class EtherFrameProvider;

class EtherFrameHandler
{
protected:
  EtherFrameProvider* backend;
  u16 ether_type_BE;

public:
  EtherFrameHandler(EtherFrameProvider* backend, u16 ether_type);
  ~EtherFrameHandler();

public:
  virtual const bool on_etherframe_recv(u8* etherframe_payload, u32 size) const;
  void send(u64 dst_MAC_BE, u8* etherframe_payload, u32 size);
};

class EtherFrameProvider : public RawDataHandler
{
  friend class EtherFrameHandler;

protected:
  EtherFrameHandler* handlers[65535];

public:
  EtherFrameProvider(amd_am79c973* backend);
  ~EtherFrameProvider();

public:
  const bool on_rawdata_recv(u8* buffer, u32 size) const;
  void send(u64 dst_MAC_BE, u16 ether_type_BE, u8* buffer, u32 size);

  const u64 get_MAC_address() const;
  const u32 get_IP_address() const;
};

#endif
