#include <cmath>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <algorithm>

#include "rsa.hpp"

using namespace std;

namespace
{
uint64_t gcd(uint64_t a, uint64_t b)
{
    return b == 0 ? a : gcd(b, a % b);
}
} // namespace

namespace RSA
{
uint64_t random_prime(string key, uint64_t size)
{
    uint64_t *S = new uint64_t[size + 1],
              k = 0, i, j;
    char* T = new char[size + 1];

    for (i = 0; i < size; i++) S[i] = i;

    for (i = j = 0; i < size; i++)
    {
        T[i] = key[j++];
        if (j == key.length()) j = 0;
    }

    for (i = j = 0; i < size; i++)
        swap(S[i], S[(j = (j + S[i] + T[i]) % size)]);

    uint64_t count = 0;
    while (true)
    {
        i = j = 0;
        i = (i + 1) % size;
        j = (j + S[i]) % size;
        swap(S[i], S[j]);

        uint64_t t = (S[i] + S[j]) % size;
        k = S[t];
        while (i < size)
        {
            if ((S[i] < k) && (S[i] > 1))
            {
                if ((k % 2 == 0)
                 || (k % 3 == 0)
                 || (k % 5 == 0)
                 || (k % 7 == 0)
                 || (k % 11 == 0)
                 || (k % 13 == 0)
                 || (k % 17 == 0)
                 || (k % 19 == 0)
                 || k % S[i] == 0)
                {
                    break;
                }
                count++;
            }
            if (count == 1000) break;
            i++;
        }
        if (count >= min((uint64_t)1000, size)) break;
    }

    return k;
}

vector<uint64_t> crypt(uint64_t n, uint64_t k, const vector<uint64_t> &input)
{
    vector<uint64_t> output = input;

    for (size_t i = 0; i < output.size(); ++i)
    {
        for (uint64_t j = 0; j < k - 1; j++)
        {
            output[i] = ((output[i] % n) * (input[i] % n)) % n;
        }
    }

    return output;
}

vector<uint64_t> get_elist(uint64_t p, uint64_t q)
{
    vector<uint64_t> elist;

    uint64_t x = (p - 1) * (q - 1);
    for (uint64_t e = x - 1; e > x - 100; e--)
    {
        if (gcd(e, x) == 1)
        {
            elist.push_back(e);
        }
    }

    return elist;
}

uint64_t choose_emax(uint64_t p, uint64_t q)
{
    uint64_t x = (p - 1) * (q - 1), e;

    for (e = x - 10;
         e > 0 && gcd(e, x) != 1;
         e--);

    return e;
}

uint64_t choose_d(uint64_t p, uint64_t q, uint64_t e)
{
    uint64_t x = (p - 1) * (q - 1), d;

    for (d = x - 1; d > 0; d--)
    {
        if (((e * d) % x) == 1 && d != e) break;
    }

    return d;
}

tuple<uint64_t, uint64_t, uint64_t> gen_ned(uint64_t prime_size, string key)
{
    uint64_t p = random_prime(key, prime_size);
    reverse(key.begin(), key.end());
    uint64_t q = random_prime(key, prime_size);
    uint64_t n = p * q;
    uint64_t e = choose_emax(p, q);
    uint64_t d = choose_d(p, q, e);
    return make_tuple(n, e, d);
}
} // namespace RSA
