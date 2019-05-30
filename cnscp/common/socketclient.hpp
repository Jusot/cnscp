#ifndef COMMON_SOCKETFUNCS_HPP
#define COMMON_SOCKETFUNCS_HPP

#include <string>
#include <netinet/in.h>

namespace cnscp
{
class SocketClient
{
  public:
    SocketClient(const std::string &address, int port);
    ~SocketClient();

    void send(const std::string &data);
    std::string recv();

  private:
    int fd_;
    sockaddr_in addr;
};
} // namespace cnscp

#endif