#include <cstring>

#include <sys/socket.h>

#include "socketserver.hpp"

using namespace std;
using namespace cnscp;

SocketServer::SocketServer(int port)
  : fd_(socket(AF_INET, SOCK_STREAM, 0))
{
    
}