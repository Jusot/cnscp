#include <cassert>
#include <cstdint>

#include <string>
#include <utility>
#include <iostream>

#include "general.hpp"
#include "../algorithms/aes.cpp"
#include "../algorithms/rsa.cpp"
#include "../algorithms/sha.cpp"
#include "common/socketclient.cpp"

using namespace std;
using namespace cnscp;

/*
Role Client

KUb, KRc, KUc : RSA
Ks            : AES
*MD           : SHA

PIMD = SHA(PI)
OIMD = SHA(OI)
DS   = E(KRc, SHA(PIMD | OIMD))

SEND : E(Ks, PI | DS | OIMD) | E(KUb, Ks) | PIMD | OI | DS | KUc
*/

string Ks(16, 0);

string gen_info(string PI, string OI, uint64_t KRc[], uint64_t KUc[], uint64_t KUb[]);

int main(int argc, char *argv[])
{
    {
        auto p = Ks.begin();
        for (auto c : "cnscp") *p++ = c;
    }

    log("Client Start...");

    log("KRc KUc Init...");
    // int KRc KUc
    auto [n, e, d] = RSA::gen_ned(kRSAPrimeMax);
    uint64_t KRc[]{n, e}, KUc[]{n, d}, KUb[2];
    cout << "[KRc KUc] [n, e, d] [" << n << ", " << e << ", " << d << "]" << endl;

    log("Receive KUb...");
    // receive KUb
    {
        SocketClient sc("127.0.0.1", kBankPort);
        sc.send("GET KUb");
        auto res = sc.recv();
        auto p = reinterpret_cast<uint64_t *>(res.data());
        for (int i = 0; i < 2; ++i) KUb[i] = *p++;
    }
    cout << "[KUb] [" << KUb[0] << ", " << KUb[1] << "]" << endl;

    while (true)
    {
        SocketClient sc("127.0.0.1", kMerchantPort);

        string PI, OI;
        cin >> PI >> OI;
        auto data = gen_info(PI, OI, KRc, KUc, KUb);

        sc.send(data);
    }

    return 0;
}

string gen_info(string PI, string OI, uint64_t KRc[], uint64_t KUc[], uint64_t KUb[])
{
    log("Generating Data...");

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
        vector<uint64_t> DS_tmp;
        for (auto c : POMD) DS_tmp.push_back(c & 0xFF);

        {
            log("DS_tmp raw: ");
            for (auto c : DS_tmp) cout << hex << c;
            cout << endl;
        }

        DS_tmp = RSA::crypt(
            KRc[0], KRc[1],
            DS_tmp
        );
        DS.insert(DS.end(), (char *)DS_tmp.data(), (char *)(DS_tmp.data() + DS_tmp.size()));

        {
            log("DS_tmp encrypted: ");
            for (auto c : DS_tmp) cout << hex << c;
            cout << endl;

            DS_tmp = RSA::crypt(KUc[0], KUc[1], DS_tmp);
            log("DS_tmp decrypted: ");
            for (auto c : DS_tmp) cout << hex << c;
            cout << endl;
        }
    }

    result += AES::_128::encrypt(PI + DS + OIMD, Ks);
    {
        auto encrypted_Ks_tmp = RSA::crypt(KUb[0], KUb[1], vector<uint64_t>(Ks.begin(), Ks.end()));
        result.insert(result.end(), (char *)encrypted_Ks_tmp.data(), (char *)(encrypted_Ks_tmp.data() + encrypted_Ks_tmp.size()));
    }
    result += PIMD + OI + DS;
    result.insert(result.end(), (char *)KUc, (char *)(KUc + 2));

    {
        log("PIMD: ");
        for (auto c : PIMD) cout << hex << (unsigned int)(unsigned char)c;
        cout << endl;
        log("OIMD: ");
        for (auto c : OIMD) cout << hex << (unsigned int)(unsigned char)c;
        cout << endl;
        log("POMD: ");
        for (auto c : POMD) cout << hex << (unsigned int)(unsigned char)c;
        cout << endl;
        log("DS  : ");
        for (auto c : DS) cout << hex << (unsigned int)(unsigned char)c;
        cout << endl;
    }

    //                kPILen | 32*8 | 32    | 8    * 16  | 32   | kOILen | 32*8 | 16
    // result = E(Ks, PI     | DS   | OIMD) | E(KUb, Ks) | PIMD | OI     | DS   | KUc
    // assert(result.size == (kPILen + 32 * 8 + 32 + 16 * 8 + 32 + kOILen + 32 * 8 + 16 ));
    assert(result.size() == kPILen + kOILen + 720);

    return result;
}
