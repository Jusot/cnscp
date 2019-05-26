#include <tuple>

#include "aes.hpp"

using namespace std;

namespace AES::_128
{
namespace
{
constexpr size_t kRounds = 10;
constexpr size_t kLength = 16;

constexpr uint8_t RC[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

constexpr uint8_t SBOX[] =
{
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
};

constexpr uint8_t INV_SBOX[] =
{
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,
};

inline uint8_t mul2(uint8_t v)
{
    return (v & 0x80)
         ? (v << 1) ^ 0x1b
         : (v << 1);
}

void shift_rows(array<uint8_t, kLength> &T)
{
    tie(T[ 1], T[ 5], T[9],  T[13],
        T[ 2], T[10], T[6],  T[14],
        T[15], T[11], T[7],  T[ 3]) = make_tuple
    (
        T[ 5], T[ 9], T[13], T[ 1],
        T[10], T[ 2], T[14], T[ 6],
        T[11], T[ 7], T[ 3], T[15]
    );
}

void inv_shift_rows(array<uint8_t, kLength> &T)
{
    tie(T[13], T[ 9], T[ 5], T[ 1],
        T[14], T[ 6], T[10], T[ 2],
        T[ 3], T[ 7], T[11], T[15]) = make_tuple
    (
        T[ 9], T[ 5], T[ 1], T[13],
        T[ 6], T[14], T[ 2], T[10],
        T[ 7], T[11], T[15], T[ 3]
    );
}

array<uint8_t, kLength * (kRounds + 1)> gen_round_keys(const array<uint8_t, kLength> &key)
{
    array<uint8_t, kLength * (kRounds + 1)> round_keys;

    auto *round_keys_ptr = round_keys.data();

    for (int i = 0; i < kLength; ++i)
        *round_keys_ptr++ = key[i];

    array<uint8_t, 4> temp;
    auto last4bytes = round_keys_ptr - 4;
    for (int i = 0; i < kRounds; ++i)
    {
        temp[3] = SBOX[*last4bytes++];
        temp[0] = SBOX[*last4bytes++];
        temp[1] = SBOX[*last4bytes++];
        temp[2] = SBOX[*last4bytes++];
        temp[0] ^= RC[i];

        auto last_round_key_ptr = round_keys_ptr - kLength;

        for (auto t : temp)
            *round_keys_ptr++ = t ^ *last_round_key_ptr++;

        for (int j = 0; j < kLength - temp.size(); ++j)
            *round_keys_ptr++ = *last4bytes++ ^ *last_round_key_ptr++;
    }

    return round_keys;
}

array<uint8_t, kLength>
encrypt_block(array<uint8_t, kLength> P, const array<uint8_t, kLength> &Key)
{

    auto K = gen_round_keys(Key);
    auto key = K.data();

    for (size_t i = 0; i < kLength; ++i)
        P[i] ^= *key++;

    for (size_t j = 1; j < kRounds; ++j)
    {
        array<uint8_t, kLength> T;

        for (int i = 0; i < kLength; ++i)
            T[i] = SBOX[P[i]];

        shift_rows(T);

        for (int i = 0; i < kLength; i += 4)
        {
            uint8_t t = T[i] ^ T[i + 1] ^ T[i + 2] ^ T[i + 3];
            P[i    ] = mul2(T[i    ] ^ T[i + 1]) ^ T[i    ] ^ t;
            P[i + 1] = mul2(T[i + 1] ^ T[i + 2]) ^ T[i + 1] ^ t;
            P[i + 2] = mul2(T[i + 2] ^ T[i + 3]) ^ T[i + 2] ^ t;
            P[i + 3] = mul2(T[i + 3] ^ T[i    ]) ^ T[i + 3] ^ t;
        }

        for (int i = 0; i < kLength; ++i)
            P[i] ^= *key++;
    }

    for (int i = 0; i < kLength; ++i)
        P[i] = SBOX[P[i]];

    shift_rows(P);

    for (int i = 0; i < kLength; ++i)
        P[i] ^= *key++;

    return P;
}

array<uint8_t, kLength>
decrypt_block(array<uint8_t, kLength> C, const array<uint8_t, kLength> &key)
{
    auto round_keys = gen_round_keys(key);
    auto round_keys_ptr = round_keys.data() + 160;

    for (int i = 0; i < kLength; ++i)
        C[i] ^= round_keys_ptr[i];

    inv_shift_rows(C);

    for (int i = 0; i < kLength; ++i)
        C[i] = INV_SBOX[C[i]];

    round_keys_ptr -= kLength;
    for (int j = 1; j < kRounds; ++j, round_keys_ptr -= 16)
    {
        array<uint8_t, kLength> T;

        for (int i = 0; i < kLength; ++i)
            T[i] = C[i] ^ round_keys_ptr[i];

        for (int i = 0; i < kLength; i += 4)
        {
            uint8_t t = T[i] ^ T[i + 1] ^ T[i + 2] ^ T[i + 3], u, v;
            C[i    ] = mul2(T[i    ] ^ T[i + 1]) ^ T[i    ] ^ t;
            C[i + 1] = mul2(T[i + 1] ^ T[i + 2]) ^ T[i + 1] ^ t;
            C[i + 2] = mul2(T[i + 2] ^ T[i + 3]) ^ T[i + 2] ^ t;
            C[i + 3] = mul2(T[i + 3] ^ T[i    ]) ^ T[i + 3] ^ t;
            u = mul2(mul2(T[i] ^ T[i + 2]));
            v = mul2(mul2(T[i + 1] ^ T[i + 3]));
            t = mul2(u ^ v);
            C[i    ] ^= t ^ u;
            C[i + 1] ^= t ^ v;
            C[i + 2] ^= t ^ u;
            C[i + 3] ^= t ^ v;
        }

        inv_shift_rows(C);

        for (int i = 0; i < kLength; ++i)
            C[i] = INV_SBOX[C[i]];
    }

    for (int i = 0; i < kLength; ++i)
        C[i] ^= round_keys_ptr[i];

    return C;
}
} // namespace

vector<uint8_t>
encrypt(const vector<uint8_t> &plaintext, const array<uint8_t, 16> &key)
{
    vector<uint8_t> result;

    vector<uint8_t> raw = plaintext;
    while (raw.size() % kLength != 0)
        raw.push_back(0);

    for (size_t i = 0; i < raw.size(); i += kLength)
    {
        array<uint8_t, kLength> group;
        for (size_t j = 0; j < kLength; ++j)
            group[j] = raw[i + j];

        auto gres = encrypt_block(group, key);
        result.insert(result.end(), gres.begin(), gres.end());
    }

    return result;
}

vector<uint8_t>
decrypt(const vector<uint8_t> &encrypted_text, const array<uint8_t, 16> &key)
{
    vector<uint8_t> result;

    vector<uint8_t> raw = encrypted_text;
    while (raw.size() % kLength != 0)
        raw.push_back(0);

    for (size_t i = 0; i < raw.size(); i += kLength)
    {
        array<uint8_t, kLength> group;
        for (size_t j = 0; j < kLength; ++j)
            group[j] = raw[i + j];

        auto gres = decrypt_block(group, key);
        result.insert(result.end(), gres.begin(), gres.end());
    }

    return result;
}

string encrypt(const string &plaintext, std::string key)
{
    array<uint8_t, kLength> k;
    for (size_t i = 0; i < kLength; ++i)
        k[i] = i < key.size() ? key[i] : 0;
    auto C = encrypt(vector<uint8_t>(plaintext.begin(), plaintext.end()), k);
    return string(C.begin(), C.end());
}

string decrypt(const string &encrypted_text, std::string key)
{
    array<uint8_t, kLength> k;
    for (size_t i = 0; i < kLength; ++i)
        k[i] = i < key.size() ? key[i] : 0;
    auto M = decrypt(vector<uint8_t>(encrypted_text.begin(), encrypted_text.end()), k);
    return string(M.begin(), M.end());
}
} // namespace AES::_128