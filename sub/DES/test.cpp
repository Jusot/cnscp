#include <iostream>

#include "des.cpp"

using namespace std;
using namespace DES;

int main()
{
    string raw = "我喜欢罗钥轩,要多喜欢有多喜欢", key = "超级喜欢";
    auto c = des(raw, key);
    auto m = des(c, key, true);

    cout << raw << endl;
    cout << key << endl;
    cout << c << endl;
    cout << m << endl;

    return 0;
}