#include <iostream>

#include "../algorithms/rsa.cpp"

using namespace RSA;

int main()
{
    uint64_t prime_size = 100000;

    uint64_t p = random_prime("bobobobobo", prime_size);
    uint64_t q = random_prime("lololololo", prime_size);
    uint64_t n = p * q;
    uint64_t e = choose_emax(p, q);
    uint64_t d = choose_d(p, q, e);

    vector<uint64_t> M { 3462,34543,2345,234,6788 };

    auto C = crypt(n, e, M);
    auto rM = crypt(n, d, C);

    {
        for (auto m: M) cout << m << " ";
        cout << endl;
        for (auto c: C) cout << c << " ";
        cout << endl;
        for (auto rm: rM) cout << rm << " ";
        cout << endl;
    }

    return 0;
}