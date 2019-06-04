#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include <cstddef>
#include <cstdint>

#include <iostream>

namespace cnscp
{
constexpr uint64_t kRSAKeyMax = 1000000;

constexpr size_t kPILen = 1024;
constexpr size_t kOILen = 1024;

constexpr int kMerchantPort = 8888;
constexpr int kBankPort = 9999;

inline void log(const string &str)
{
    std::cout << str << std::endl;
}
}

#endif
