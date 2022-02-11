#include "net/icmp.hpp"

ICMP::ICMP(IPProvider* backend)
  : IPHandler(backend, 0x01)
{}

ICMP::~ICMP() {}

const bool
ICMP::on_ip_recv(u32 src_IP_BE, u32 dst_IP_BE, u8* ip_payload, u32 size) const
{
  if (size < sizeof(ICMPMessage)) {
    return false;
  }

  ICMPMessage* msg = (ICMPMessage*)ip_payload;

  switch (msg->type) {
    case 0:
      printf("ping response from ");
      printh(src_IP_BE & 0xFF);
      printf(".");
      printh((src_IP_BE >> 8) & 0xFF);
      printf(".");
      printh((src_IP_BE >> 16) & 0xFF);
      printf(".");
      printh((src_IP_BE >> 24) & 0xFF);
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
ICMP::request_echo_reply(u32 ip_be)
{
  ICMPMessage icmp;
  icmp.type = 8;
  icmp.code = 0;
  icmp.data = 0x3713;
  icmp.checksum = 0;
  icmp.checksum = IPProvider::checksum((u16*)&icmp, sizeof(ICMPMessage));
  IPHandler::send(ip_be, (u8*)&icmp, sizeof(ICMPMessage));
}
