#include <iostream>

#include "sha.cpp"

#define TEST(str) cout << str << endl; for (auto i : sha256(str)) cout << hex << i; cout << endl;

using namespace std;
using namespace SHA;

int main()
{
    TEST("woaini");
    return 0;
}