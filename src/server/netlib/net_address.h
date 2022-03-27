/**
 * @file net_address.h
 * @brief
 * @date 2022-02-27
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_SERVER_NETLIB_NET_ADDRESS_H_
#define MRPROXYER_SRC_SERVER_NETLIB_NET_ADDRESS_H_

#include "platform.h"

namespace netlib {

class NetAddress {
 public:
  NetAddress() = default;
  NetAddress(const std::string& ip, uint16_t port);
  NetAddress(std::string ip, int16_t port, bool ipv6);

  std::string GetIp();
  uint16_t GetPort();
  std::string GetIpAndPort();

  std::any GetAddress();
  uint32_t GetSize();

 private:
  bool ipv6_ = false;
  union {
    struct sockaddr_in in4_;
    struct sockaddr_in6 in6_;
  };
};
}  // namespace netlib

#endif  // MRPROXYER_SRC_SERVER_NETLIB_NET_ADDRESS_H_
