#include <iostream>
#include <vector>

#include "rc4.hpp"

using namespace std;

namespace RC4
{
RC4::RC4(int n, string key)
{
	S_ = new int[n]();
	T_ = new char[n]();

	for (int i = 0; i < 256; i++)
		S_[i] = i;

	for (int i = 0, j = 0; i < 256; i++)
	{
		T_[i] = key[j++];
		if (j == key.length()) j = 0;
	}

	for (int i = 0, j = 0; i < 256; i++)
		swap(S_[i], S_[(j = (j + S_[i] + T_[i]) % 256)]);
}

RC4::~RC4()
{
	delete[] S_;
	delete[] T_;
}

int RC4::gen_one()
{
	p_ = (p_ + 1) % 256;
	q_ = (q_ + S_[p_]) % 256;
	swap(S_[p_], S_[q_]);
	int t = (S_[p_] + S_[q_]) % 256;
	return S_[t];
}

vector<int> RC4::gen_list()
{
	vector<int> result;

	for (int i = 0; i < 256; ++i)
		result.push_back(S_[i]);

	for (int i = 0, j = 0; i < 256; i++)
		swap(S_[i], S_[(j = (j + S_[i] + T_[i]) % 256)]);

	return result;
}
} // namespace RC4