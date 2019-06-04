#include <cassert>

#include <tuple>
#include <vector>
#include <iostream>

#include "general.hpp"
#include "../algorithms/sha.cpp"
#include "../algorithms/rsa.cpp"
#include "common/socketfuncs.hpp"
#include "common/socketclient.cpp"
#include "common/socketserver.cpp"

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
        for (int i = 0; i < 2; ++i) KUc[i] = *p++;
    }

    {
        cout << "[KUc] [" << KUc[0] << ", " << KUc[1] << "]" << endl;
    }

    auto OIMD = SHA::sha256(OI);
    auto POMD = SHA::sha256(PIMD + OIMD);
    string POMD_recv;
    {
        vector<uint64_t> temp((uint64_t *)DS.data(), (uint64_t *)(DS.data() + DS.size()));
        temp = RSA::crypt(KUc[0], KUc[1], temp);
        for (auto t : temp) POMD_recv.push_back(t & 0xFF);
    }

    {
        log("POMD: ");
        for (auto c : POMD) cout << hex << (unsigned int)(unsigned char)c;
        cout << endl;
        log("DS_recv: ");
        for (auto c : DS) cout << hex << (unsigned int)(unsigned char)c;
        cout << endl;
        log("POMD_recv: ");
        for (auto c : POMD_recv) cout << hex << (unsigned int)(unsigned char)c;
        cout << endl;
    }

    if (POMD == POMD_recv) return make_tuple(true, DS);
    else return make_tuple(false, DS);
}

void process(int fd)
{
    auto data = sf::recv(fd);

    auto d4b = data.substr(0, kPILen + 416),
         d4m = data.substr(kPILen + 416, kOILen + 304);

    auto [matched, DS] = check_data(d4m);
    if (matched)
    {
        SocketClient sc("127.0.0.1", kBankPort);

        sc.send(d4b + d4m.substr(kOILen + 288, 16) + DS);
        auto res = sc.recv();
        if (res.substr(0, 7) == "SUCCESS") log("DS is same to Bank's");
        else log("DS is different from Bank's");
    }
    else log("Calculated DS can't match the raw data");
}
