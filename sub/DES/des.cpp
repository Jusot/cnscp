#include <tuple>
#include <bitset>
#include <cassert>

#include "des.hpp"

using namespace std;

namespace
{
constexpr size_t ip_table[] =
{
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7,
};

constexpr size_t expa_perm[] =
{
    32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,  8,  9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1,
};

constexpr uint8_t sbox[][64] =
{
    {
        14,  4, 13, 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9, 0,  7,
         0, 15,  7, 4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5, 3,  8,
         4,  1, 14, 8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10, 5,  0,
        15, 12,  8, 2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0, 6, 13,
    },
    {
        15,  1,  8, 14,  6, 11,  3,  4,  9, 7,  2, 13, 12, 0,  5, 10,
         3, 13,  4,  7, 15,  2,  8, 14, 12, 0,  1, 10,  6, 9, 11,  5,
         0, 14,  7, 11, 10,  4, 13,  1,  5, 8, 12,  6,  9, 3,  2, 15,
        13,  8, 10,  1,  3, 15,  4,  2, 11, 6,  7, 12,  0, 5, 14,  9,
    },
    {
        10,  0,  9, 14, 6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
        13,  7,  0,  9, 3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
        13,  6,  4,  9, 8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
         1, 10, 13,  0, 6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
    },
    {
         7, 13, 14, 3,  0,  6,  9, 10,  1, 2, 8,  5, 11, 12,  4, 15,
        13,  8, 11, 5,  6, 15,  0,  3,  4, 7, 2, 12,  1, 10, 14,  9,
        10,  6,  9, 0, 12, 11,  7, 13, 15, 1, 3, 14,  5,  2,  8,  4,
         3, 15,  0, 6, 10,  1, 13,  8,  9, 4, 5, 11, 12,  7,  2, 14,
    },
    {
         2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13, 0, 14,  9,
        14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3, 9,  8,  6,
         4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6, 3,  0, 14,
        11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10, 4,  5,  3,
    },
    {
        12,  1, 10, 15, 9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
        10, 15,  4,  2, 7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
         9, 14, 15,  5, 2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
         4,  3,  2, 12, 9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
    },
    {
         4, 11,  2, 14, 15, 0,  8, 13,  3, 12, 9,  7,  5, 10, 6,  1,
        13,  0, 11,  7,  4, 9,  1, 10, 14,  3, 5, 12,  2, 15, 8,  6,
         1,  4, 11, 13, 12, 3,  7, 14, 10, 15, 6,  8,  0,  5, 9,  2,
         6, 11, 13,  8,  1, 4, 10,  7,  9,  5, 0, 15, 14,  2, 3, 12,
    },
    {
        13,  2,  8, 4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
         1, 15, 13, 8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
         7, 11,  4, 1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
         2,  1, 14, 7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11,
    }
};

constexpr size_t p_table[] =
{
    16, 7, 20, 21, 29, 12, 28, 17,  1, 15, 23, 26,  5, 18, 31, 10,
     2, 8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6, 22, 11,  4, 25,
};

constexpr size_t ipr_table[] =
{
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25,
};

constexpr size_t key_table[] =
{
    57, 49, 41, 33, 25, 17,  9,  1,
    58, 50, 42, 34, 26, 18, 10,  2,
    59, 51, 43, 35, 27, 19, 11,  3,
    60, 52, 44, 36, 63, 55, 47, 39,
    31, 23, 15,  7, 62, 54, 46, 38,
    30, 22, 14,  6, 61, 53, 45, 37,
    29, 21, 13,  5, 28, 20, 12,  4,
};

constexpr size_t bit_shift[] =
{
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

constexpr size_t comp_perm[] =
{
    14, 17, 11, 24,  1,  5,  3, 28,
    15,  6, 21, 10, 23, 19, 12,  4,
    26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32,
};

array<bool, 64> str2bits(string str)
{
    while (str.size() < 8) str.push_back(0);

    array<bool, 64> result;
    for (size_t i = 0, j = 0; i < 64; i += 8, ++j)
    {
        result[i    ] = str[j] & 0b10000000;
        result[i + 1] = str[j] & 0b01000000;
        result[i + 2] = str[j] & 0b00100000;
        result[i + 3] = str[j] & 0b00010000;
        result[i + 4] = str[j] & 0b00001000;
        result[i + 5] = str[j] & 0b00000100;
        result[i + 6] = str[j] & 0b00000010;
        result[i + 7] = str[j] & 0b00000001;
    }
    return result;
}

string bits2str(const array<bool, 64> &bits)
{
    string result;
    for (int i = 0; i < 64; i += 8)
    {
        result.push_back((bits[i    ] << 7) |
                         (bits[i + 1] << 6) |
                         (bits[i + 2] << 5) |
                         (bits[i + 3] << 4) |
                         (bits[i + 4] << 3) |
                         (bits[i + 5] << 2) |
                         (bits[i + 6] << 1) |
                         bits[i + 7]);
    }
    return result;
}

array<bool, 48> extend(array<bool, 32> input)
{
    array<bool, 48> result;
    for (size_t i = 0; i < 48; ++i)
        result[i] = input[expa_perm[i] - 1];
    return result;
}

array<bool, 32> sbox_proc(array<bool, 48> input)
{
    array<bool, 32> result;

    bitset<2> row;
    bitset<4> col;

    for (size_t i = 0; i < 8; ++i)
    {
        row[0] = input[i * 6 + 5];
        row[1] = input[i * 6 + 0];

        col[0] = input[i * 6 + 4];
        col[1] = input[i * 6 + 3];
        col[2] = input[i * 6 + 2];
        col[3] = input[i * 6 + 1];

        bitset<4> temp(sbox[i][row.to_ulong() * 16 + col.to_ulong()]);
        for (size_t j = 0; j < 4; ++j)
            result[i * 4 + 3 - j] = temp[j];
    }

    return result;
}

array<bool, 32> pbox_proc(array<bool, 32> input)
{
    array<bool, 32> res;
    for (size_t i = 0; i < 32; ++i)
        res[i] = input[p_table[i] - 1];
    return res;
}

array<bool, 64> ipr_proc(array<bool, 64> input)
{
    array<bool, 64> res;
    for (size_t i = 0; i < 64; ++i)
        res[i] = input[ipr_table[i] - 1];
    return res;
}

array<array<bool, 48>, 17> gen_keys(array<bool, 56> key)
{
    array<array<bool, 48>, 17> keys;

    bool lkeys[17][28], rkeys[17][28];

    for (int i = 0; i < 28; i++)
    {
        lkeys[0][i] = key[i];
        rkeys[0][i] = key[i + 28];
    }

    for (size_t round = 1; round < 17; ++round)
    {
        for (size_t i = 0; i < 28; ++i)
        {
            lkeys[round][i] = lkeys[round - 1][(i + bit_shift[round - 1]) % 28];
            rkeys[round][i] = rkeys[round - 1][(i + bit_shift[round - 1]) % 28];
        }
    }

    for (size_t j = 0; j < 17; ++j)
    {
        for (size_t i = 0; i < 48; ++i)
        {
            auto ind = comp_perm[i] - 1;
            keys[j][i] = (ind < 28)
                       ? lkeys[j][ind]
                       : rkeys[j][ind - 28];
        }
    }

    return keys;
}
} // namespace

namespace DES
{
array<bool, 64> des(array<bool, 64> M, array<bool, 64> key, bool encrypt = true)
{
    array<bool, 64> T;
    array<bool, 56> K;
    array<array<bool, 32>, 17> L, R;

    for (size_t i = 0; i < 64; ++i)
        T[i] = M[ip_table[i] - 1];

    for (int i = 0; i < 32; i++)
    {
        L[0][i] = T[i];
        R[0][i] = T[i + 32];
    }

    for (size_t i = 0; i < 56; ++i)
        K[i] = key[key_table[i] - 1];

    auto keys = gen_keys(K);
    if (!encrypt)
    {
        for (int i = 1; i < 9; ++i)
        {
            swap(keys[i], keys[17 - i]);
        }
    }

    for (int j = 1; j < 17; j++)
    {
        auto t1 = encrypt ? extend(R[j - 1]) : extend(L[j - 1]);
        for (size_t i = 0; i < 48; ++i)
            t1[i] = t1[i] ^ keys[j][i];
        auto t2 = pbox_proc(sbox_proc(t1));
        for (size_t i = 0; i < 32; ++i)
        {
            if (encrypt)
            {
                R[j][i] = t2[i] ^ L[j - 1][i];
                L[j][i] = R[j - 1][i];
            }
            else
            {
                L[j][i] = t2[i] ^ R[j - 1][i];
                R[j][i] = L[j - 1][i];
            }
        }
    }

    for (int i = 0; i < 32; ++i)
    {
        T[i] = L[16][i];
        T[i + 32] = R[16][i];
    }

    return ipr_proc(T);
}

string des(string M, std::string key, bool encrypt = true)
{
    string result;

    auto k = str2bits(key);

    while (M.size() % 8 != 0)
        M.push_back(0);

    for (size_t i = 0; i < M.size(); i += 8)
    {
        auto group = M.substr(i, 8);
        result += bits2str(des(str2bits(group), k, encrypt));
    }

    return result;
}
} // namespace DES