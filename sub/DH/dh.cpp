#include <tuple>
#include <string>

using namespace std;

namespace DH
{
int ramdom_int(int n, string key)
{
    int *S = new int[n](), i, j;
    char *T = new char[n]();

    for (i = 0; i < n; i++)
    {
        S[i] = i;
    }

    for (i = 0, j = 0; i < n; i++)
    {
        if (j == key.length()) j = 0;
        else T[i] = key[j++];
    }

    for (i = 0, j = 0; i < n; i++)
    {
        swap(S[i], S[(j = (j + S[i] + T[i]) % n)]);
    }

    i = j = 0;
    i = (i + 1) % n;
    j = (j + S[i]) % n;
    swap(S[i], S[j]);

    return S[(S[i] + S[j]) % n];
}

int DH_Y(int a, int q, int X)
{
    int Y = a;
    for (int i = 1; i < X; i++)
    {
        Y = Y * Y % q;
    }
    return Y;
}

int DH_S(int q, int X, int Y)
{
    int S = Y;
    for (int i = 1; i < X; i++)
    {
        S = S * S % q;
    }
    return S;
}
} // namespace DH