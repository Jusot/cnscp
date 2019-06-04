#include <iostream>

#include "../algorithms/sha.cpp"

#define TEST(str) cout << str << endl; for (auto i : sha256(str)) cout << hex << (unsigned int)(unsigned char)i; cout << endl;

using namespace std;
using namespace SHA;

int main()
{
    TEST("woaini luoyuexuan");
    return 0;
}
