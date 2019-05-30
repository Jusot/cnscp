#ifndef SHA_HPP
#define SHA_HPP

#include <array>
#include <string>
#include <vector>
#include <cstdint>

namespace SHA
{
std::array<uint32_t, 8>
sha256(const std::vector<uint8_t> &raw);

std::array<uint32_t, 8>
sha256(const std::string &raw);
} // namespace SHA

#endif // SHA_HPP