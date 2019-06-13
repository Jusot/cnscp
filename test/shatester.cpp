#include <iostream>

#include "../algorithms/sha.cpp"

#define TEST(str) cout << str << endl; for (auto i : sha256(str)) cout << hex << (unsigned int)(unsigned char)i; cout << endl;

using namespace std;
using namespace SHA;

int main()
{
    TEST("wo you yi zhi xiao mao lv wo cong lai ye bu qi");
    return 0;
}
