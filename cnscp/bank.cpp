#include <tuple>
#include <iostream>

#include "general.hpp"
#include "../algorithms/rsa.hpp"
#include "common/socketfuncs.hpp"
#include "common/socketserver.hpp"

using namespace std;
using namespace cnscp;
namespace sf = socketfuncs;

/*
Role Bank

KRb, KUb : RSA

Receive : E(Ks, PI | DS | OIMD) | E(KUb, Ks)
*/

uint64_t KRb[2], KUb[2];

void process(int fd);

int main(int argc, char *argv[])
{
    log("Bank Start...");

    log("KRb KUB Init...");
    // init KRb KUB
    auto [n, e, d] = RSA::gen_ned(kRSAKeyMax);
    KRb[0] = n; KRb[1] = e;
    KUb[0] = n; KUb[1] = d;
    cout << "[KRb KUb] [n, e, d] [" << n << ", " << e << ", " << d << "]" << endl;

    log("Bank Server Init...");
    SocketServer ss(kBankPort, process);
    log("Bank Server Run...");
    ss.run();

    return 0;
}

tuple<bool, string> check_data(const string &data)
{
    // TODO: check
}

void process(int fd)
{
    auto data = sf::recv(fd);
    if (data.substr(0, 7) == "GET KUb")
    {
        log("GET KUb");
        sf::send(fd, string((char*)KUb, (char*)(KUb + 2)));
    }
    else
    {
        auto [matched, DS] = check_data(data);
        if (matched)
        {
            // TODO: print successful matched info

            sf::send(fd, DS);
        }
        else
        {
            sf::send(fd, "Match Failed");
        }
    }
}
