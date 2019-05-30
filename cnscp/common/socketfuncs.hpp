#ifndef COMMON_SOCKETFUNCS_HPP
#define COMMON_SOCKETFUNCS_HPP

#include <string>

#include <unistd.h>

namespace cnscp
{
namespace socketfuncs
{
inline void send(int fd, const std::string &data)
{
    write(fd, data.c_str(), data.size() + 1);
}

inline std::string recv(int fd)
{
    char buf[4096] = { 0 };
    read(fd, buf, 4096);
    return buf;
}
} // namespace socketfuncs
} // namespace cnscp

#endif // COMMON_SOCKETFUNCS_HPP