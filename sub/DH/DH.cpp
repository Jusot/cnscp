#include <tuple>
#include <string>
#include <iostream>

using namespace std;

int RC4(int n, string key)
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

int main()
{
	int a = 3, q = 23;

	auto [Xa, Xb] = make_tuple(RC4(q, "lolo"), RC4(q, "bobo"));
	auto [Ya, Yb] = make_tuple(DH_Y(a, q, Xa), DH_Y(a, q, Xb));
	auto [Sa, Sb] = make_tuple(DH_S(q, Xa, Yb), DH_S(q, Xb, Ya));

	cout << "Xa: " << Xa << endl;
	cout << "Ya: " << Ya << endl;
	cout << "Sa: " << Sa << endl;
	cout << "Xb: " << Xb << endl;
	cout << "Yb: " << Yb << endl;
	cout << "Sb: " << Sb << endl;

    return 0;
}