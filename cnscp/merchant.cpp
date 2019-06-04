#include <tuple>
#include <iostream>

#include "general.hpp"
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
Send    to   Bank   : | (Length)kPILen + 416 | DS
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
    // TODO: check
}

void process(int fd)
{
    auto data = sf::recv(fd);
    SocketClient sc("127.0.0.1", kBankPort);

    auto [d4b, d4m] = make_tuple(data.substr(0, kPILen + 416), data.substr(kPILen + 416));
    auto [matched, DS] = check_data(d4m);

    if (matched)
    {
        sc.send(d4b + DS);
        auto res = sc.recv();
        if (res.substr(0, 7) == "SUCCESS") log("DS is same to Bank's");
        else log("DS is different from Bank's");
    }
    else log("Calculated DS can't match the raw data");
}
