#include "net/icmp.hpp"

ICMP::ICMP(IPProvider* backend)
  : IPHandler(backend, 0x01)
{}

ICMP::~ICMP() {}

const bool
ICMP::on_ip_recv(const u32 src_ip_be,
                 const u32 dst_ip_be,
                 const u8* ip_payload,
                 const u32 size) const
{
  if (size < sizeof(ICMPMessage)) {
    return false;
  }

  ICMPMessage* msg = (ICMPMessage*)ip_payload;

  switch (msg->type) {
    case 0:
      printf("ping response from ");
      printh(src_ip_be & 0xFF);
      printf(".");
      printh((src_ip_be >> 8) & 0xFF);
      printf(".");
      printh((src_ip_be >> 16) & 0xFF);
      printf(".");
      printh((src_ip_be >> 24) & 0xFF);
      printf("\n");
      break;

    case 8:
      msg->type = 0;
      msg->checksum = 0;
      msg->checksum = IPProvider::checksum((u16*)msg, sizeof(ICMPMessage));
      return true;
  }

  return false;
}

void
ICMP::request_echo_reply(const u32 ip_be)
{
  ICMPMessage icmp;
  icmp.type = 8;
  icmp.code = 0;
  icmp.data = 0x3713;
  icmp.checksum = 0;
  icmp.checksum = IPProvider::checksum((u16*)&icmp, sizeof(ICMPMessage));
  IPHandler::send(ip_be, (u8*)&icmp, sizeof(ICMPMessage));
}
