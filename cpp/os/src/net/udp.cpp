#include "net/udp.hpp"
#include "memory.hpp"

UDPHandler::UDPHandler() {}

UDPHandler::~UDPHandler() {}

void
UDPHandler::handle_udp_message(UDPSocket* socket, u8* data, u16 size)
{}

UDPSocket::UDPSocket(UDPProvider* $backend)
{
  backend = $backend;
  handler = 0;
  listening = false;
}

UDPSocket::~UDPSocket() {}

void
UDPSocket::handle_udp_message(u8* data, u16 size)
{
  if (handler != 0) {
    handler->handle_udp_message(this, data, size);
  }
}

void
UDPSocket::send(u8* data, u16 size)
{
  backend->send(this, data, size);
}

void
UDPSocket::disconnect()
{
  backend->disconnect(this);
}

UDPProvider::UDPProvider(IPProvider* backend)
  : IPHandler(backend, 0x11)
{
  for (u32 i = 0; i < 65535; i++) {
    sockets[i] = 0;
  }
  total_sockets = 0;
  free_port = 1024;
}

UDPProvider::~UDPProvider() {}

const bool
UDPProvider::on_ip_recv(u32 src_ip_be,
                        u32 dst_ip_be,
                        u8* ip_payload,
                        u32 size) const
{
  if (size < sizeof(UDPHeader)) {
    return false;
  }

  UDPHeader* msg = (UDPHeader*)ip_payload;
  u16 local_port = msg->dst_port;
  u16 remote_port = msg->src_port;

  UDPSocket* socket = 0;
  for (u16 i = 0; i < total_sockets && socket == 0; i++) {
    if (sockets[i]->local_port == msg->dst_port &&
        sockets[i]->local_ip == dst_ip_be && sockets[i]->listening) {
      socket = sockets[i];
      socket->listening = false;
      socket->remote_port = msg->src_port;
      socket->remote_ip = src_ip_be;
    } else if (sockets[i]->local_port == msg->dst_port &&
               sockets[i]->local_ip == dst_ip_be &&
               sockets[i]->remote_port == msg->src_port &&
               sockets[i]->remote_ip == src_ip_be) {
      socket = sockets[i];
    }
  }

  if (socket != 0) {
    socket->handle_udp_message(ip_payload + sizeof(UDPHeader),
                               size - sizeof(UDPHeader));
  }

  return false;
}

UDPSocket*
UDPProvider::connect(u32 ip, u16 port)
{
  UDPSocket* socket =
    (UDPSocket*)MemoryManager::active_memory_manager->malloc(sizeof(UDPSocket));

  if (socket != 0) {
    socket = new UDPSocket(this);

    socket->remote_port = port;
    socket->remote_ip = ip;
    socket->local_port = free_port++;
    socket->local_ip = backend->get_ip_address();

    socket->remote_port = ((socket->remote_port & 0xFF00) >> 8) |
                          ((socket->remote_port & 0x00FF) << 8);
    socket->local_port = ((socket->local_port & 0xFF00) >> 8) |
                         ((socket->local_port & 0x00FF) << 8);

    sockets[total_sockets++] = socket;
  }

  return socket;
}

UDPSocket*
UDPProvider::listen(u16 port)
{
  UDPSocket* socket =
    (UDPSocket*)MemoryManager::active_memory_manager->malloc(sizeof(UDPSocket));

  if (socket != 0) {
    socket = new UDPSocket(this);

    socket->listening = true;
    socket->local_port = port;
    socket->local_ip = backend->get_ip_address();

    socket->local_port = ((socket->local_port & 0xFF00) >> 8) |
                         ((socket->local_port & 0x00FF) << 8);

    sockets[total_sockets++] = socket;
  }

  return socket;
}

void
UDPProvider::disconnect(UDPSocket* socket)
{
  for (u16 i = 0; i < total_sockets && socket == 0; i++) {
    if (sockets[i] == socket) {
      sockets[i] = sockets[--total_sockets];
      MemoryManager::active_memory_manager->free(socket);
      break;
    }
  }
}

void
UDPProvider::send(UDPSocket* socket, u8* data, u16 size)
{
  u16 total_length = size + sizeof(UDPHeader);
  u8* buffer = (u8*)MemoryManager::active_memory_manager->malloc(total_length);
  u8* buffer2 = buffer + sizeof(UDPHeader);

  UDPHeader* msg = (UDPHeader*)buffer;
  msg->src_port = socket->local_port;
  msg->dst_port = socket->remote_port;
  msg->length = ((total_length & 0x00FF) << 8) | ((total_length & 0xFF00) >> 8);

  for (int i = 0; i < size; i++) {
    buffer2[i] = data[i];
  }

  msg->checksum = 0;
  IPHandler::send(socket->remote_ip, buffer, total_length);

  MemoryManager::active_memory_manager->free(buffer);
}

void
UDPProvider::bind(UDPSocket* socket, UDPHandler* handler)
{
  socket->handler = handler;
}
