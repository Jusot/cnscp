#ifndef COMMON_SOCKETFUNCS_HPP
#define COMMON_SOCKETFUNCS_HPP

#include <string>

namespace cnscp
{
namespace socketfuncs
{
void send(int fd, const std::string &data);
std::string recv(int fd);
} // namespace socketfuncs
} // namespace cnscp

#endif // COMMON_SOCKETFUNCS_HPP