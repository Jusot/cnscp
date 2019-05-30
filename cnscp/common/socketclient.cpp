#include <cstring>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

#include "socketclient.hpp"

#define ERRPROC abort()

using namespace std;
using namespace cnscp;

SocketClient::SocketClient(const string &address, int port)
  : fd_(socket(AF_INET, SOCK_STREAM, 0))
{
    auto host = gethostbyname(address.c_str());
    if (host == nullptr) ERRPROC;

    bzero(&addr, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = *((in_addr *)host->h_addr);

    auto err = connect(fd_, (sockaddr *)&addr, sizeof(sockaddr));
    if (err) ERRPROC;
}

SocketClient::~SocketClient()
{
    close(fd_);
}

void SocketClient::send(const string &data)
{
    write(fd_, data.c_str, data.size() + 1);
}

string SocketClient::recv()
{
    char buf[4096] = { 0 };
    read(fd_, buf, 4096);
    return buf;
}