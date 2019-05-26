#ifndef DH_HPP
#define DH_HPP

#include <string>

namespace DH
{
int random_int(int n, std::string key);
int DH_Y(int a, int q, int X);
int DH_S(int q, int X, int Y);
} // namespace DH

#endif // DH_HPP