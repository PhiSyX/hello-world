#ifndef __UDP_HPP__
#define __UDP_HPP__

#include "net/ipv4.hpp"
#include "types.hpp"

// User Datagram Protocol

struct UDPHeader
{
  u16 src_port;
  u16 dst_port;
  u16 length;
  u16 checksum;
} __attribute__((packed));

class UDPSocket;
class UDPProvider;

class UDPHandler
{
public:
  UDPHandler();
  ~UDPHandler();

public:
  virtual void handle_udp_message(UDPSocket* socket, u8* data, u16 size);
};

class UDPSocket
{
  friend class UDPProvider;

protected:
  u16 remote_port;
  u32 remote_ip;
  u16 local_port;
  u32 local_ip;
  UDPProvider* backend;
  UDPHandler* handler;
  bool listening;

public:
  UDPSocket(UDPProvider* backend);
  ~UDPSocket();

public:
  virtual void handle_udp_message(u8* data, u16 size);
  virtual void send(u8* data, u16 size);
  virtual void disconnect();
};

class UDPProvider : IPHandler
{
protected:
  UDPSocket* sockets[65535];
  u16 total_sockets;
  u16 free_port;

public:
  UDPProvider(IPProvider* backend);
  ~UDPProvider();

  virtual bool on_ip_recv(u32 src_IP_BE,
                          u32 dst_IP_BE,
                          u8* ip_payload,
                          u32 size);

  virtual UDPSocket* connect(u32 ip, u16 port);
  virtual UDPSocket* listen(u16 port);
  virtual void disconnect(UDPSocket* socket);
  virtual void send(UDPSocket* socket, u8* data, u16 size);

  virtual void bind(UDPSocket* socket, UDPHandler* handler);
};

#endif
