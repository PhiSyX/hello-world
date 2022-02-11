#ifndef __TCP_HPP__
#define __TCP_HPP__

#include "net/ipv4.hpp"
#include "types.hpp"

// Transmission Control Protocol

enum TCPSocketState
{
  CLOSED,
  LISTEN,
  SYN_SENT,
  SYN_RECEIVED,

  ESTABLISHED,

  FIN_WAIT1,
  FIN_WAIT2,
  CLOSING,
  TIME_WAIT,

  CLOSE_WAIT,
  //   LAST_ACK,
};

enum TCPFlag
{
  FIN = 1,
  SYN = 2,
  RST = 4,
  PSH = 8,
  ACK = 16,
  URG = 32,
  ECE = 64,
  CWR = 128,
  NS = 256
};

struct TCPHeader
{
  u16 src_port;
  u16 dst_port;
  u32 sequence_number;
  u32 acknowledgement_number;

  u8 reserved : 4;
  u8 header_size_32 : 4;
  u16 flags;

  u16 window_size;
  u16 checksum;
  u16 urgent_ptr;

  u32 options;
} __attribute__((packed));

struct TCPPseudoHeader
{
  u32 src_ip;
  u32 dst_ip;
  u16 protocol;
  u16 total_length;
} __attribute__((packed));

class TCPSocket;
class TCPProvider;

class TCPHandler
{
public:
  TCPHandler();
  ~TCPHandler();

public:
  virtual const bool handle_tcp_message(const TCPSocket* socket,
                                        u8* data,
                                        u16 size) const;
};

class TCPSocket
{
  friend class TCPProvider;

protected:
  u16 remote_port;
  u32 remote_ip;
  u16 local_port;
  u32 local_ip;
  u32 acknowledgement_number;
  u32 sequence_number;
  TCPProvider* backend;
  TCPHandler* handler;
  TCPSocketState state;

public:
  TCPSocket(TCPProvider* backend);
  ~TCPSocket();

public:
  virtual const bool handle_tcp_message(u8* data, u16 size) const;
  virtual void send(u8* data, u16 size);
  virtual void disconnect();
};

class TCPProvider : IPHandler
{
protected:
  TCPSocket* sockets[65535];
  u16 total_sockets;
  u16 free_port;

public:
  TCPProvider(IPProvider* backend);
  ~TCPProvider();

public:
  virtual bool on_ip_recv(u32 src_ip_be,
                          u32 dst_ip_be,
                          u8* ip_payload,
                          u32 size);

  virtual TCPSocket* connect(u32 ip, u16 port);
  virtual void disconnect(TCPSocket* socket);
  virtual void send(TCPSocket* socket, u8* data, u16 size, u16 flags = 0);

  virtual TCPSocket* listen(u16 port);
  virtual void bind(TCPSocket* socket, TCPHandler* handler);
};

#endif
