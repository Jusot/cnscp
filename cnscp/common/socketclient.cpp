#include <cstring>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

#include "socketfuncs.hpp"
#include "socketclient.hpp"

#define ERRPROC abort()

using namespace std;
using namespace cnscp;

SocketClient::SocketClient(const string &address, int port)
  : fd_(socket(AF_INET, SOCK_STREAM, 0))
{
    auto host = gethostbyname(address.c_str());
    if (host == nullptr) ERRPROC;

    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr = *((in_addr *)host->h_addr);

    auto err = connect(fd_, (sockaddr *)&addr_, sizeof(sockaddr));
    if (err) ERRPROC;
}

SocketClient::~SocketClient()
{
    close(fd_);
}

void SocketClient::send(const string &data)
{
    socketfuncs::send(fd_, data);
}

string SocketClient::recv()
{
    return socketfuncs::recv(fd_);
}