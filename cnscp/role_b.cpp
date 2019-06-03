#include <iostream>

#include "constant.hpp"
#include "../algorithms/rsa.hpp"
#include "common/socketfuncs.hpp"
#include "common/socketserver.hpp"

using namespace std;
using namespace cnscp;
namespace sf = socketfuncs;

/*
ROLE B

KRb, KUb : RSA
*/

uint64_t KRb[2], KUb[2];

void process(int fd);

int main(int argc, char *argv[])
{
    cout << "Bank Start..." << endl;

    cout << "KRb KUB Init..." << endl;
    // init KRb KUB
    auto [n, e, d] = RSA::gen_ned(kRSAKeyMax);
    KRb[0] = n; KRb[1] = e;
    KUb[0] = n; KUb[1] = d;
    cout << "[KRb KUb] [n, e, d] [" << n << ", " << e << ", " << d << "]" << endl;

    SocketServer ss(9999, process);
    ss.run();

    return 0;
}

void process(int fd)
{
    auto data = sf::recv(fd);
    if (data.substr(0, 7) == "GET KUb")
    {
        cout << "GET KUb" << endl;
        sf::send(fd, string((char*)KUb, (char*)(KUb + 2)));
    }
    else
    {
        // ...
    }
}