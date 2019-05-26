#include <iostream>

#include "dh.cpp"

using namespace DH;

int main()
{
    int a = 3, q = 23;

    auto [Xa, Xb] = make_tuple(ramdom_int(q, "lolo"), ramdom_int(q, "bobo"));
    auto [Ya, Yb] = make_tuple(DH_Y(a, q, Xa), DH_Y(a, q, Xb));
    auto [Sa, Sb] = make_tuple(DH_S(q, Xa, Yb), DH_S(q, Xb, Ya));

    cout << "Xa: " << Xa << endl;
    cout << "Ya: " << Ya << endl;
    cout << "Sa: " << Sa << endl;
    cout << "Xb: " << Xb << endl;
    cout << "Yb: " << Yb << endl;
    cout << "Sb: " << Sb << endl;

    return 0;
}