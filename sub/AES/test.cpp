#include <iostream>

#include "aes.cpp"

using namespace std;
using namespace AES;

int main()
{
    string M = "woaini";
    string key = "luoyuexuan";

    auto C = encrypt::AES128(M, key);
    auto Res = decrypt::AES128(C, key);

    cout << C << endl;
    cout << Res << endl;
    
    return 0;
}