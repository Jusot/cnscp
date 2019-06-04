#include <tuple>
#include <iostream>

#include "constant.hpp"
#include "common/socketfuncs.hpp"
#include "common/socketclient.hpp"
#include "common/socketserver.hpp"

using namespace std;
using namespace cnscp;
namespace sf = socketfuncs;

/*
Role Merchant

Only receive from Client

Receive : | (Length)kPILen + 416 | PIMD | OI | DS | KUc
Send    : | (Length)kPILen + 416 |
*/

void process(int fd);

int main(int argc, char *argv[])
{
    cout << "Merchant Start..." << endl;

    cout << "Merchant Server Init..." << endl;
    SocketServer ss(kMerchantPort, process);
    cout << "Merchant Server Run..." << endl;
    ss.run();

    return 0;
}

tuple<bool, string> check_data(const string &data)
{

}

void process(int fd)
{
    auto data = sf::recv(fd);
    SocketClient sc("127.0.0.1", kBankPort);

    auto [d4m, d4b] = make_tuple(data.substr(0, kPILen + 416), data.substr(kPILen + 416));

    auto [matched, DS] = check_data(d4m);

    if (matched)
    {
        sc.send(d4m);
        auto DS_from_bank = sc.recv();
    }
    else
    {
        // TODO: process when not matched
    }
}
