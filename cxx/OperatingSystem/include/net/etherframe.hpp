#ifndef __ETHERFRAME_HPP__
#define __ETHERFRAME_HPP__

#include "drivers/AMD/am79c973.hpp"
#include "types.hpp"

struct EtherFrameHeader
{
  u64 dst_mac_be : 48;
  u64 src_mac_be : 48;
  u16 ether_type_be;
} __attribute__((packed));

typedef u32 EtherFrameFooter;

class EtherFrameProvider;

class EtherFrameHandler
{
protected:
  EtherFrameProvider* backend;
  u16 ether_type_be;

public:
  EtherFrameHandler(EtherFrameProvider* backend, const u16 ether_type);
  ~EtherFrameHandler();

public:
  virtual const bool on_etherframe_recv(const u8* etherframe_payload,
                                        const u32 size) const;
  void send(const u64 dst_mac_be, const u8* etherframe_payload, const u32 size);
  const u32 get_ip_address() const;
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
  const bool on_rawdata_recv(const u8* buffer, const u32 size) const;
  void send(const u64 dst_mac_be,
            const u16 ether_type_be,
            const u8* buffer,
            const u32 size);

  const u64 get_mac_address() const;
  const u32 get_ip_address() const;
};

#endif
