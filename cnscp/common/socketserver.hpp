#ifndef COMMON_SOCKETSERVER_HPP
#define COMMON_SOCKETSERVER_HPP

#include <string>
#include <functional>
#include <netinet/in.h>

namespace cnscp
{
class SocketServer
{
  public:
    using Callback = std::function<void(int)>;

    SocketServer(int port, Callback cb);
    ~SocketServer();

    void run();

  private:
    int fd_;
    sockaddr_in addr_;
    Callback callback_;
};
} // namespace cnscp

#endif // COMMON_SOCKETSERVER_HPP