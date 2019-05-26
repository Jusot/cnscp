#ifndef AES_HPP
#define AES_HPP

#include <array>
#include <vector>
#include <string>
#include <cstdint>

namespace AES
{
namespace encrypt
{
std::vector<std::uint8_t>
AES128(const std::vector<std::uint8_t> &plaintext, const std::array<std::uint8_t, 16> &key);

std::vector<std::uint8_t>
AES128(const std::string &plaintext, const std::string &key);
} // namespace encrypt

namespace decrypt
{
std::vector<std::uint8_t>
AES128(const std::vector<std::uint8_t> &plaintext, const std::array<std::uint8_t, 16> &key);

std::vector<std::uint8_t>
AES128(const std::string &plaintext, const std::string &key);
} // namespace decrypt
} // namespace AES

#endif // AES_HPP