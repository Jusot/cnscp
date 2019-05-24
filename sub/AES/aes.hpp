#include <array>
#include <vector>
#include <cstdint>

namespace AES
{
namespace encrypt
{
std::vector<std::uint32_t>
AES128(const std::vector<std::uint32_t> &plaintext, const std::array<std::uint32_t, 4> &key);

std::vector<std::uint32_t>
AES192(const std::vector<std::uint32_t> &plaintext, const std::array<std::uint32_t, 6> &key);

std::vector<std::uint32_t>
AES256(const std::vector<std::uint32_t> &plaintext, const std::array<std::uint32_t, 8> &key);
} // namespace encrypt

namespace decrypt
{
std::vector<std::uint32_t>
AES128(const std::vector<std::uint32_t> &plaintext, const std::array<std::uint32_t, 4> &key);

std::vector<std::uint32_t>
AES192(const std::vector<std::uint32_t> &plaintext, const std::array<std::uint32_t, 6> &key);

std::vector<std::uint32_t>
AES256(const std::vector<std::uint32_t> &plaintext, const std::array<std::uint32_t, 8> &key);
} // namespace decrypt
} // namespace AES