#include <cstdint>

#include <string>
#include <utility>
#include <iostream>

#include "../algorithms/aes.hpp"
#include "../algorithms/rsa.hpp"
#include "../algorithms/sha.hpp"
#include "common/socketclient.hpp"

using namespace std;
using namespace cnscp;

/*
ROLE C

KUb, KRc, KUc : RSA
Ks            : AES
*MD           : SHA

PIMD = SHA(PI)
OIMD = SHA(OI)
DS = E(KRc, SHA(PIMD | OIMD))
E(Ks, PI | DS | OIMD) | E(KUb, Ks) | PIMD | OI | DS | KUc
*/

string Ks = "cnscp";

string gen_info(const string &PI, const string &OI, uint64_t KRc[], uint64_t KUc[], uint64_t KUb[])
{
    string result;

    string PIMD, OIMD, POMD, DS;

    PIMD = SHA::sha256(PI); 
    OIMD = SHA::sha256(OI);
    POMD = SHA::sha256(PIMD + OIMD);
    {
        auto DS_tmp = RSA::crypt(KRc[0], KRc[1], vector<uint64_t>(POMD.begin(), POMD.end()));
        for (auto ui : DS_tmp)
            for (int i = 56; i >= 0; i -= 8)
                DS.push_back((ui >> i) & 0xFF);
    }

    result += AES::_128::encrypt(PI + DS + OIMD, Ks);
    {
        auto temp = RSA::crypt(KUb[0], KUb[1], vector<uint64_t>(Ks.begin(), Ks.end()));
        for (auto ui : temp)
            for (int i = 56; i >= 0; i -= 8)
                result.push_back((ui >> i) & 0xFF);
    }
    result += PIMD + OI + DS;
    {
        auto p = reinterpret_cast<char *>(KUc);
        for (int i = 0; i < 8; ++i) result.push_back(*p++);
    }
    
    return result;
}

int main(int argc, char *argv[])
{
    // int KRc KUc
    auto [n, e, d] = RSA::gen_ned(1000000);
    uint64_t KRc[]{n, e}, KUc[]{n, d}, KUb[2];

    // receive KUb
    {
        SocketClient sc("127.0.0.1", 8888);
        sc.send("GET KU");
        auto res = sc.recv();
        auto p = reinterpret_cast<uint64_t *>(res.data());
        for (int i = 0; i < 2; ++i) KUb[i++] = *p++;
    }

    SocketClient sc("127.0.0.1", 7777);
    while (true)
    {
        string PI, OI;
        cin >> PI >> OI;
        auto data = gen_info(PI, OI, KRc, KUc, KUb);
        sc.send(data);
    }

    return 0;
}