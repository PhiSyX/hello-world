#include "net/tcp.hpp"
#include "memory.hpp"

TCPHandler::TCPHandler() {}

TCPHandler::~TCPHandler() {}

const bool
TCPHandler::handle_tcp_message(const TCPSocket* socket,
                               u8* data,
                               u16 size) const
{
  return true;
}

TCPSocket::TCPSocket(TCPProvider* backend)
{
  this->backend = backend;
  handler = 0;
  state = TCPSocketState::CLOSED;
}

TCPSocket::~TCPSocket() {}

const bool
TCPSocket::handle_tcp_message(u8* data, u16 size) const
{
  if (handler != 0) {
    return handler->handle_tcp_message(this, data, size);
  }
  return false;
}

void
TCPSocket::send(u8* data, u16 size)
{
  backend->send(this, data, size);
}

void
TCPSocket::disconnect()
{
  backend->disconnect(this);
}

TCPProvider::TCPProvider(IPProvider* backend)
  : IPHandler(backend, 0x06)
{
  for (int i = 0; i < 65535; i++) {
    sockets[i] = 0;
  }
  total_sockets = 0;
  free_port = 1024;
}

TCPProvider::~TCPProvider() {}

constexpr u32
big_endian_32(u32 x)
{
  return (x & 0xFF000000) >> 24 | (x & 0x00FF0000) >> 8 |
         (x & 0x0000FF00) << 8 | (x & 0x000000FF) << 24;
}

bool
TCPProvider::on_ip_recv(u32 src_ip_be, u32 dst_ip_be, u8* ip_payload, u32 size)
{

  if (size < 20) {
    return false;
  }

  TCPHeader* msg = (TCPHeader*)ip_payload;

  u16 local_port = msg->dst_port;
  u16 remote_port = msg->src_port;

  TCPSocket* socket = 0;
  for (u16 i = 0; i < total_sockets && socket == 0; i++) {
    if (sockets[i]->local_port == msg->dst_port &&
        sockets[i]->local_ip == dst_ip_be &&
        sockets[i]->state == TCPSocketState::LISTEN &&
        ((msg->flags) & (TCPFlag::SYN | TCPFlag::ACK) == TCPFlag::SYN)) {
      socket = sockets[i];
    } else if (sockets[i]->local_port == msg->dst_port &&
               sockets[i]->local_ip == dst_ip_be &&
               sockets[i]->remote_port == msg->src_port &&
               sockets[i]->remote_ip == src_ip_be) {
      socket = sockets[i];
    }
  }

  bool reset = false;

  if (socket != 0 && msg->flags & TCPFlag::RST) {
    socket->state = TCPSocketState::CLOSED;
  }

  if (socket != 0 && socket->state != TCPSocketState::CLOSED) {
    switch ((msg->flags) & (TCPFlag::SYN | TCPFlag::ACK | TCPFlag::FIN)) {
      case TCPFlag::SYN:
        if (socket->state == TCPSocketState::LISTEN) {
          socket->state = TCPSocketState::SYN_RECEIVED;
          socket->remote_port = msg->src_port;
          socket->remote_ip = src_ip_be;
          socket->acknowledgement_number =
            big_endian_32(msg->sequence_number) + 1;
          socket->sequence_number = 0xbeefcafe;
          send(socket, 0, 0, TCPFlag::SYN | TCPFlag::ACK);
          socket->sequence_number++;
        } else {
          reset = true;
        }
        break;

      case TCPFlag::SYN | TCPFlag::ACK:
        if (socket->state == TCPSocketState::SYN_SENT) {
          socket->state = TCPSocketState::ESTABLISHED;
          socket->acknowledgement_number =
            big_endian_32(msg->sequence_number) + 1;
          socket->sequence_number++;
          send(socket, 0, 0, TCPFlag::ACK);
        } else {
          reset = true;
        }
        break;

      case TCPFlag::SYN | TCPFlag::FIN:
      case TCPFlag::SYN | TCPFlag::FIN | TCPFlag::ACK:
        reset = true;
        break;

      case TCPFlag::FIN:
      case TCPFlag::FIN | TCPFlag::ACK:
        if (socket->state == TCPSocketState::ESTABLISHED) {
          socket->state = TCPSocketState::CLOSE_WAIT;
          socket->acknowledgement_number++;
          send(socket, 0, 0, TCPFlag::ACK);
          send(socket, 0, 0, TCPFlag::FIN | TCPFlag::ACK);
        } else if (socket->state == TCPSocketState::CLOSE_WAIT) {
          socket->state = TCPSocketState::CLOSED;
        } else if (socket->state == TCPSocketState::FIN_WAIT1 ||
                   socket->state == TCPSocketState::FIN_WAIT2) {
          socket->state = TCPSocketState::CLOSED;
          socket->acknowledgement_number++;
          send(socket, 0, 0, TCPFlag::ACK);
        } else {
          reset = true;
        }
        break;

      case TCPFlag::ACK:
        if (socket->state == TCPSocketState::SYN_RECEIVED) {
          socket->state = TCPSocketState::ESTABLISHED;
          return false;
        } else if (socket->state == TCPSocketState::FIN_WAIT1) {
          socket->state = TCPSocketState::FIN_WAIT2;
          return false;
        } else if (socket->state == TCPSocketState::CLOSE_WAIT) {
          socket->state = TCPSocketState::CLOSED;
          return false;
        }

        if (msg->flags == TCPFlag::ACK) {
          break;
        }

      default:

        if (big_endian_32(msg->sequence_number) ==
            socket->acknowledgement_number) {
          reset =
            !(socket->handle_tcp_message(ip_payload + msg->header_size_32 * 4,
                                         size - msg->header_size_32 * 4));
          if (!reset) {
            socket->acknowledgement_number += size - msg->header_size_32 * 4;
            send(socket, 0, 0, TCPFlag::ACK);
          }
        } else {
          reset = true;
        }
    }
  }

  if (reset) {
    return true;
  }

  if (socket != 0 && socket->state == TCPSocketState::CLOSED) {
    for (u16 i = 0; i < total_sockets && socket == 0; i++) {
      if (sockets[i] == socket) {
        sockets[i] = sockets[--total_sockets];
        MemoryManager::active_memory_manager->free(socket);
        break;
      }
    }
  }

  return false;
}

void
TCPProvider::send(TCPSocket* socket, u8* data, u16 size, u16 flags)
{
  u16 total_length = size + sizeof(TCPHeader);
  u16 length_phdr = total_length + sizeof(TCPPseudoHeader);

  u8* buffer = (u8*)MemoryManager::active_memory_manager->malloc(length_phdr);

  TCPPseudoHeader* phdr = (TCPPseudoHeader*)buffer;
  TCPHeader* msg = (TCPHeader*)(buffer + sizeof(TCPPseudoHeader));
  u8* buffer2 = buffer + sizeof(TCPHeader) + sizeof(TCPPseudoHeader);

  msg->header_size_32 = sizeof(TCPHeader) / 4;
  msg->src_port = socket->local_port;
  msg->dst_port = socket->remote_port;

  msg->acknowledgement_number = big_endian_32(socket->acknowledgement_number);
  msg->sequence_number = big_endian_32(socket->sequence_number);
  msg->reserved = 0;
  msg->flags = flags;
  msg->window_size = 0xFFFF;
  msg->urgent_ptr = 0;

  msg->options = ((flags & TCPFlag::SYN) != 0) ? 0xB4050402 : 0;

  socket->sequence_number += size;

  for (int i = 0; i < size; i++) {
    buffer2[i] = data[i];
  }

  phdr->src_ip = socket->local_ip;
  phdr->dst_ip = socket->remote_ip;
  phdr->protocol = 0x0600;
  phdr->total_length =
    ((total_length & 0x00FF) << 8) | ((total_length & 0xFF00) >> 8);

  msg->checksum = 0;
  msg->checksum = IPProvider::checksum((u16*)buffer, length_phdr);

  IPHandler::send(socket->remote_ip, (u8*)msg, total_length);
  MemoryManager::active_memory_manager->free(buffer);
}

TCPSocket*
TCPProvider::connect(u32 ip, u16 port)
{
  TCPSocket* socket =
    (TCPSocket*)MemoryManager::active_memory_manager->malloc(sizeof(TCPSocket));

  if (socket != 0) {
    socket = new TCPSocket(this);

    socket->remote_port = port;
    socket->remote_ip = ip;
    socket->local_port = free_port++;
    socket->local_ip = backend->get_ip_address();

    socket->remote_port = ((socket->remote_port & 0xFF00) >> 8) |
                          ((socket->remote_port & 0x00FF) << 8);
    socket->local_port = ((socket->local_port & 0xFF00) >> 8) |
                         ((socket->local_port & 0x00FF) << 8);

    sockets[total_sockets++] = socket;
    socket->state = TCPSocketState::SYN_SENT;

    socket->sequence_number = 0xbeefcafe;

    send(socket, 0, 0, TCPFlag::SYN);
  }

  return socket;
}

void
TCPProvider::disconnect(TCPSocket* socket)
{
  socket->state = TCPSocketState::FIN_WAIT1;
  send(socket, 0, 0, TCPFlag::FIN + TCPFlag::ACK);
  socket->sequence_number++;
}

TCPSocket*
TCPProvider::listen(u16 port)
{
  TCPSocket* socket =
    (TCPSocket*)MemoryManager::active_memory_manager->malloc(sizeof(TCPSocket));

  if (socket != 0) {
    socket = new TCPSocket(this);

    socket->state = TCPSocketState::LISTEN;
    socket->local_ip = backend->get_ip_address();
    socket->local_port = ((port & 0xFF00) >> 8) | ((port & 0x00FF) << 8);

    sockets[total_sockets++] = socket;
  }

  return socket;
}
void

TCPProvider::bind(TCPSocket* socket, TCPHandler* handler)
{
  socket->handler = handler;
}
