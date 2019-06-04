#include <cassert>

#include <tuple>
#include <vector>
#include <iostream>

#include "general.hpp"
#include "../algorithms/sha.hpp"
#include "../algorithms/rsa.hpp"
#include "common/socketfuncs.hpp"
#include "common/socketclient.hpp"
#include "common/socketserver.hpp"

using namespace std;
using namespace cnscp;
namespace sf = socketfuncs;

/*
Role Merchant

Only receive from Client

Receive from Client : | (Length)kPILen + 416 | PIMD | OI | DS | KUc
Send    to   Bank   : | (Length)kPILen + 416 | KUc | DS
*/

void process(int fd);

int main(int argc, char *argv[])
{
    log("Merchant Start...");

    log("Merchant Server Init...");
    SocketServer ss(kMerchantPort, process);
    log("Merchant Server Run...");
    ss.run();

    return 0;
}

tuple<bool, string> check_data(const string &data)
{
    auto PIMD = data.substr(0, 32),
         OI   = data.substr(32, kOILen),
         DS   = data.substr(kOILen + 32, 256);
    uint64_t KUc[2];
    {
        auto temp = data.substr(kOILen + 288, 16);
        auto p = reinterpret_cast<uint64_t *>(temp.data());
        for (int i = 0; i < 2; ++i) KUc[i++] = *p++;
    }

    auto OIMD = SHA::sha256(OI);
    auto POMD = SHA::sha256(PIMD + OIMD);
    string POMD_recv;
    {
        vector<uint64_t> temp;
        for (int i = 0; i < 32; ++i)
        {
            temp.push_back
            (
                (DS[i * 8 + 0] << 56) |
                (DS[i * 8 + 1] << 48) |
                (DS[i * 8 + 2] << 40) |
                (DS[i * 8 + 3] << 32) |
                (DS[i * 8 + 4] << 24) |
                (DS[i * 8 + 5] << 16) |
                (DS[i * 8 + 6] <<  8) |
                (DS[i * 8 + 7])
            );
        }
        temp = RSA::crypt(KUc[0], KUc[1], temp);
        for (auto t : temp) POMD_recv.push_back(t & 0xFF);
    }

    if (POMD == POMD_recv) return make_tuple(true, DS);
    else return make_tuple(false, DS);
}

void process(int fd)
{
    auto data = sf::recv(fd);
    SocketClient sc("127.0.0.1", kBankPort);

    auto d4b = data.substr(0, kPILen + 416),
         d4m = data.substr(kPILen + 416);

    auto [matched, DS] = check_data(d4m);
    if (matched)
    {
        sc.send(d4b + d4m.substr(kOILen + 288, 16) + DS);
        auto res = sc.recv();
        if (res.substr(0, 7) == "SUCCESS") log("DS is same to Bank's");
        else log("DS is different from Bank's");
    }
    else log("Calculated DS can't match the raw data");
}
