#include <tuple>
#include <iostream>

#include "general.hpp"
#include "../algorithms/rsa.hpp"
#include "../algorithms/sha.hpp"
#include "../algorithms/aes.hpp"
#include "common/socketfuncs.hpp"
#include "common/socketserver.hpp"

using namespace std;
using namespace cnscp;
namespace sf = socketfuncs;

/*
Role Bank

KRb, KUb : RSA

Receive : E(Ks, PI | DS | OIMD) | E(KUb, Ks) | KUc | DS
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
    auto encrypted_PI_DS_OIMD = data.substr(0, kPILen + 288),
         encrypted_Ks = data.substr(kPILen + 288, 128);
    uint64_t KUc[2];
    {
        auto temp = data.substr(kPILen + 416, 16);
        auto p = reinterpret_cast<uint64_t *>(temp.data());
        for (int i = 0; i < 2; ++i) KUc[i++] = *p++;
    }

    string Ks;
    {
        vector<uint64_t> temp;
        for (int i = 0; i < 16; ++i)
        {
            temp.push_back
            (
                (encrypted_Ks[i * 8 + 0] << 56) |
                (encrypted_Ks[i * 8 + 1] << 48) |
                (encrypted_Ks[i * 8 + 2] << 40) |
                (encrypted_Ks[i * 8 + 3] << 32) |
                (encrypted_Ks[i * 8 + 4] << 24) |
                (encrypted_Ks[i * 8 + 5] << 16) |
                (encrypted_Ks[i * 8 + 6] <<  8) |
                (encrypted_Ks[i * 8 + 7])
            );
        }
        temp = RSA::crypt(KRb[0], KRb[1], temp);
        for (auto t : temp) Ks.push_back(t & 0xFF);
    }

    auto PI_DS_OIMD = AES::_128::decrypt(encrypted_PI_DS_OIMD, Ks);
    auto PI   = PI_DS_OIMD.substr(0, kPILen),
         DS   = PI_DS_OIMD.substr(kPILen, 256),
         OIMD = PI_DS_OIMD.substr(kPILen + 256, 32);

    auto PIMD = SHA::sha256(PI);
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
    if (data.substr(0, 7) == "GET KUb")
    {
        log("GET KUb");
        sf::send(fd, string((char*)KUb, (char*)(KUb + 2)));
    }
    else
    {
        auto raw = data.substr(0, kPILen + 432);
        auto DS_recv = data.substr(kPILen + 432, 256);

        auto [matched, DS] = check_data(raw);
        if (matched && DS == DS_recv)
        {
            log("DS is matched!");
            sf::send(fd, "SUCCESS");
        }
        else
        {
            log("DS is not matched~");
            sf::send(fd, "FAILED");
        }
    }
}
