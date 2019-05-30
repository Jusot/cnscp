#ifndef AES_HPP
#define AES_HPP

#include <array>
#include <vector>
#include <string>
#include <cstdint>

namespace AES
{
namespace _128
{
std::vector<std::uint8_t>
encrypt(const std::vector<std::uint8_t> &plaintext, const std::array<std::uint8_t, 16> &key);

std::string
encrypt(const std::string &plaintext, std::string key);

std::vector<std::uint8_t>
decrypt(const std::vector<std::uint8_t> &plaintext, const std::array<std::uint8_t, 16> &key);

std::string
decrypt(const std::string &plaintext, std::string key);
} // namespace _128
} // namespace AES

#endif // AES_HPP