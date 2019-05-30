#ifndef RC4_HPP
#define RC4_HPP

#include <vector>
#include <string>

namespace RC4
{
class RC4
{
public:
    RC4(int n, std::string key);
    ~RC4();

    int gen_one();
    std::vector<int> gen_list();

private:
    int p_ = 0;
    int q_ = 0;
    int *S_;
    char *T_;
};
} // namespace RC4

#endif // RC4_HPP