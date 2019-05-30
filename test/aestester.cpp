#include <iostream>

#include "../algorithms/aes.cpp"

using namespace std;
namespace AES128 = AES::_128;

int main()
{
    string M = "woaini";
    string key = "luoyuexuan";

    auto C = AES128::encrypt(M, key);
    auto Res = AES128::decrypt(C, key);

    cout << C << endl;
    cout << Res << endl;

    return 0;
}