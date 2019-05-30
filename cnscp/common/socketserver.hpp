#ifndef COMMON_SOCKETSERVER_HPP
#define COMMON_SOCKETSERVER_HPP

#include <string>
#include <netinet/in.h>

namespace cnscp
{
class SocketServer
{
  public:
    SocketServer(int port);
    ~SocketServer();

    void listen();
    void run();

  private:
    int fd_;
    sockaddr_in addr_;

    void send(const std::string &data);
    std::string recv();
};
} // namespace cnscp

#endif // COMMON_SOCKETSERVER_HPP