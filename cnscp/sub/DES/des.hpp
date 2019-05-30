#ifndef DES_HPP
#define DES_HPP

#include <array>
#include <string>

namespace DES
{
std::array<bool, 64> DES(std::array<bool, 64> M, std::array<bool, 64> key);
std::string DES(std::string M, std::string key, bool encrypt = true);
} // namespace DES

#endif