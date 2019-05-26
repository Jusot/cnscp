#ifndef RSA_HPP
#define RSA_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace RSA
{
uint64_t random_prime(std::string key, uint64_t size);
uint64_t choose_emax(uint64_t p, uint64_t q);
uint64_t choose_d(uint64_t p, uint64_t q, uint64_t e);
std::vector<uint64_t> get_elist(uint64_t p, uint64_t q);
std::vector<uint64_t> crypt(uint64_t n, uint64_t k, const std::vector<uint64_t> &input);
} // namespace RSA

#endif // RSA_HPP