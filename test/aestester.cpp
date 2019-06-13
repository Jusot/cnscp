#include <iostream>

#include "../algorithms/aes.cpp"

using namespace std;
namespace AES128 = AES::_128;

int main()
{
    string M = "hahaha";
    string key = "hehehe";

    auto C = AES128::encrypt(M, key);
    auto Res = AES128::decrypt(C, key);

    for (auto c : C) cout << hex << (unsigned int)(unsigned char)c; cout << endl;
    cout << Res << endl;

    return 0;
}