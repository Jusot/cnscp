#include <tuple>
#include <bitset>

#include "sha.hpp"

using namespace std;

namespace
{
constexpr uint32_t K[] =
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

vector<uint32_t>
padding_and_append_len(vector<uint8_t> input)
{
    auto len = input.size() * 8;

    // step 1
    input.push_back(0b10000000U);
    while (input.size() % 64 != 56)
        input.push_back(0);
    
    // step 2
    uint64_t tail = len;
    uint8_t *tmp = reinterpret_cast<uint8_t *>(&tail);
    for (int i = 0; i < 8; ++i)
        input.push_back(*tmp++);

    vector<uint32_t> res;
    auto temp = reinterpret_cast<uint32_t *>(input.data());
    for (int i = 0; i < input.size() / 4; ++i)
        res.push_back(*temp++);
    return res;
}

uint32_t rotate(uint32_t val, int n)
{
    return (val >> n) | (val << (32 - n));
}

uint32_t shift(uint32_t val, int n)
{
    return val >> n;
}

uint32_t condition(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & y) ^ ((~x) & z));
}

uint32_t majority(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & y) ^ (x & z) ^ (y & z));
}

uint32_t sigma0(uint32_t x)
{
    return rotate(x, 2) ^ rotate(x, 13) ^ rotate(x, 22);
}

uint32_t sigma1(uint32_t x)
{
    return rotate(x, 6) ^ rotate(x, 11) ^ rotate(x, 25);
}

uint32_t delta0(uint32_t x)
{
    return rotate(x, 7) ^ rotate(x, 18) ^ shift(x, 3);
}

uint32_t delta1(uint32_t x)
{
    return rotate(x, 17) ^ rotate(x, 19) ^ shift(x, 10);
}
} // namespace

namespace SHA
{
array<uint32_t, 8>
sha256(const vector<uint8_t> &raw)
{
    auto input = padding_and_append_len(raw);

    array<uint32_t, 8> res =
    {
        0x6A09E667,
        0xBB67AE85,
        0x3C6EF372,
        0xA54FF53A,
        0x510E527F,
        0x9B05688C,
        0x1F83D9AB,
        0x5BE0CD19
    };

    for (size_t i = 0; i < input.size(); i += 16)
    {
        array<uint32_t, 16> M;
        for (auto j = i; j < i + 16; ++j)
            M[j] = input[i + j];

        auto [A, B, C, D, E, F, G, H] = res;

        array<uint32_t, 64> W;
        for (int i = 0; i < 16; ++i)
            W[i] = M[i];
        for (int i = 16; i < 64; ++i)
            W[i] = delta1(W[i - 2]) + W[i - 7] + delta0(W[i - 15]) + W[i - 16];

        for (int i = 0; i < 64; ++i)
        {
            auto [T1, T2] = make_tuple
            (
                H + sigma1(E) + condition(E, F, G) + K[i] + W[i],
                sigma0(A) + majority(A, B, C)
            );
            tie(H, G, F, E, D, C, B, A) = make_tuple
            (
                G, F, E, D + T1, C, B, A, T1 + T2
            );
        }

        array<uint32_t, 8> vs = { A, B, C, D, E, F, G, H };
        for (size_t i = 0; i < 8; ++i)
            res[i] += vs[i];
    }

    return res;
}

array<uint32_t, 8>
sha256(const string &raw)
{
    return sha256(vector<uint8_t>(raw.begin(), raw.end()));
}
} // namespace SHA