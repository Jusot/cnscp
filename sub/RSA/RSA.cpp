#include <cmath>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <iostream>

using namespace std;

uint64_t gcd(uint64_t a, uint64_t b)
{
	return b > 0 ? gcd(b, a % b) : a;
}

uint64_t random_prime(string key, uint64_t size)
{
	uint64_t *S = new uint64_t[size + 1],
			  k = 0, i, j;
	char* T = new char[size + 1];

	for (i = 0; i < size; i++) S[i] = i;

	for (i = j = 0; i < size; i++)
	{
		T[i] = key[j++];
		if (j == key.length()) j = 0;
	}

	for (i = j = 0; i < size; i++)
		swap(S[i], S[(j = (j + S[i] + T[i]) % size)]);

	uint64_t count = 0;
	while (true)
	{
		i = j = 0;
		i = (i + 1) % size;
		j = (j + S[i]) % size;
		swap(S[i], S[j]);

		uint64_t t = (S[i] + S[j]) % size;	
		k = S[t];
		while (i < size)
		{
			if ((S[i] < k) && (S[i] > 1))
			{
				if ((k % 2 == 0) 
				 || (k % 3 == 0) 
				 || (k % 5 == 0) 
				 || (k % 7 == 0) 
				 || (k % 11 == 0) 
				 || (k % 13 == 0) 
				 || (k % 17 == 0) 
				 || (k % 19 == 0) 
				 || k % S[i] == 0)
				{
					break;
				}
				count++;
			}
			if (count == 1000) break;
			i++;
		}
		if (count == 1000) break;
	}

	return k;
}

vector<uint64_t> encrypt(uint64_t n, uint64_t e, const vector<uint64_t> &M)
{
	vector<uint64_t> C = M;

	for (size_t i = 0; i < M.size(); ++i)
	{
		for (uint64_t j = 0; j < e - 1; j++)
		{
			C[i] = ((C[i] % n) * (M[i] % n)) % n;
		}
	}

	return C;
}

vector<uint64_t> decrypt(uint64_t n, uint64_t d, const vector<uint64_t> &C)
{
	vector<uint64_t> M = C;

	for (size_t i = 0; i < M.size(); ++i)
	{
		for (uint64_t j = 0; j < d - 1; j++)
		{
			M[i] = ((M[i] % n) * (C[i] % n)) % n;
		}
	}

	return M;
}

vector<uint64_t> get_elist(uint64_t p, uint64_t q)
{
	vector<uint64_t> elist;

	uint64_t x = (p - 1) * (q - 1);
	for (uint64_t e = x - 1; e > x - 100; e--)
	{
		if (gcd(e, x) == 1)
		{
			elist.push_back(e);
		}
	}

	return elist;
}

uint64_t choose_emax(uint64_t p, uint64_t q)
{
	uint64_t x = (p - 1) * (q - 1), e;

	for (e = x - 1;
		 e > x - 100 && gcd(e, x) != 1;
		 e--);

	return e;
}

uint64_t choose_d(uint64_t p, uint64_t q, uint64_t e)
{
	uint64_t x = (p - 1) * (q - 1), d;

	for (d = x - 1; d > 0; d--)
	{
		if ((e * d) % x == 1) break;
	}

	return d;
}

int main()
{
	uint64_t prime_size = 100000;

	uint64_t p = random_prime("bobobobobo", prime_size);
	uint64_t q = random_prime("lololololo", prime_size);
	uint64_t n = p * q;
	uint64_t e = choose_emax(p, q);
	uint64_t d = choose_d(p, q, e);

	vector<uint64_t> M { 3462,34543,2345,234,6788 };

	auto C = encrypt(n, e, M);
	auto rM = decrypt(n, d, C);

	{
		for (auto m: M) cout << m << " ";
		cout << endl;
		for (auto c: C) cout << c << " ";
		cout << endl;
		for (auto rm: rM) cout << rm << " ";
		cout << endl;
	}

	return 0;
}