#include <iostream>

#include "aes.hpp"

#define TEST(str, key) cout << str << endl; for (auto c : encrypt::AES128(str, key)) cout << hex << c; cout << endl;

using namespace std;
using namespace AES;

int main()
{
    TEST("woaini", "luoyuexuan");

    return 0;
}