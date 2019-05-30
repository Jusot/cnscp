#include <cstring>

#include <thread>
#include <unistd.h>
#include <sys/socket.h>

#include "socketserver.hpp"

#define ERRPROC abort()

using namespace std;
using namespace cnscp;

SocketServer::SocketServer(int port, Callback cb)
  : fd_(socket(AF_INET, SOCK_STREAM, 0)),
    callback_(std::move(cb))
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);

    auto err = bind(fd_, (sockaddr *)&addr_, sizeof(sockaddr));
    if (err) ERRPROC;
}

SocketServer::~SocketServer()
{
    close(fd_);
}

void SocketServer::run()
{
    auto err = ::listen(fd_, 5);
    if (err) ERRPROC;

    while (true)
    {
        sockaddr_in target_addr;
        socklen_t length;
        auto target_fd = accept(fd_, (sockaddr *)&target_addr, &length);
        if (target_fd == -1) ERRPROC;
        std::thread t([this]
        {
            this->callback_(this->fd_);
        });
        t.detach();
    }
}