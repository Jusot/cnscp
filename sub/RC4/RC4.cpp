#include <iostream>
#include <vector>
using namespace std;

class RC4
{
public:
	RC4(int n, string key);
	~RC4();
	vector<int> list();
	int get();
	int n() const
	{
		return n_;
	}

private:
	int n_;
	int p_ = 0;
	int q_ = 0;
	int *S_;
	char *T_;
};

RC4::RC4(int n, string key)
  : n_(n)
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

vector<int> RC4::list()
{
	vector<int> result;

	for (int i = 0; i < 256; ++i)
		result.push_back(S_[i]);

	for (int i = 0, j = 0; i < 256; i++)
		swap(S_[i], S_[(j = (j + S_[i] + T_[i]) % 256)]);

	return result;
}

int RC4::get()
{
	p_ = (p_ + 1) % 256;
	q_ = (q_ + S_[p_]) % 256;
	swap(S_[p_], S_[q_]);
	int t = (S_[p_] + S_[q_]) % 256;
	return S_[t];
}