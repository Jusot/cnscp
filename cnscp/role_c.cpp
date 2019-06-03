#include <cassert>
#include <cstdint>

#include <string>
#include <utility>
#include <iostream>

#include "constant.hpp"
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

string Ks(16, 0);

string gen_info(string PI, string OI, uint64_t KRc[], uint64_t KUc[], uint64_t KUb[]);

int main(int argc, char *argv[])
{
    {
        auto p = Ks.begin();
        for (auto c : "cnscp") *p++ = c;
    }

    cout << "Client Start..." << endl;

    cout << "KRc KUc Init..." << endl;
    // int KRc KUc
    auto [n, e, d] = RSA::gen_ned(kRSAKeyMax);
    uint64_t KRc[]{n, e}, KUc[]{n, d}, KUb[2];
    cout << "[KRc KUc] [n, e, d] [" << n << ", " << e << ", " << d << "]" << endl;

    cout << "Receive KUb..." << endl;
    // receive KUb
    {
        SocketClient sc("127.0.0.1", 8888);
        sc.send("GET KUb");
        auto res = sc.recv();
        auto p = reinterpret_cast<uint64_t *>(res.data());
        for (int i = 0; i < 2; ++i) KUb[i++] = *p++;
    }
    cout << "[KUb] [" << KUb[0] << ", " << KUb[1] << "]" << endl;

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

string gen_info(string PI, string OI, uint64_t KRc[], uint64_t KUc[], uint64_t KUb[])
{
    string result;

    string PIMD, OIMD, POMD, DS;

    while (PI.size() < kPILen) PI.push_back(0);
    while (OI.size() < kOILen) OI.push_back(0);

    assert(PI.size() == kPILen && OI.size() == kOILen);

    PIMD = SHA::sha256(PI);
    OIMD = SHA::sha256(OI);

    assert(PIMD.size() == 32 && OIMD.size() == 32);

    POMD = SHA::sha256(PIMD + OIMD);
    {
        assert(POMD.size() == 32);
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

    //                kPILen | 32*8 | 32    | 8    * 16  | 32   | kOILen | 32*8 | 8
    // result = E(Ks, PI     | DS   | OIMD) | E(KUb, Ks) | PIMD | OI     | DS   | KUc
    // assert(result.size == (kPILen + 32 * 8 + 32 + 16 * 8 + 32 + kOILen + 32 * 8 + 8 ));
    assert(result.size() == kPILen + kOILen + 712);

    return result;
}