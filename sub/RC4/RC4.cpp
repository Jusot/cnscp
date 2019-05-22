// RC4.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

using namespace std;

class RC4
{
public:
	RC4();
	~RC4();
	int RC4_swap();
	int out();

private:
	int S[256];
	char T[256];
	string key;
	int k = 0;
};

RC4::RC4()
{
	memset(S, 256, sizeof(S));
	memset(T, 256, sizeof(T));
	int i;
	for (i = 0; i < 256; i++)
	{
		S[i] = i;
	}
	cout << "Please enter the key：" << endl;
	cin >> key;
	int j = 0;
	int len = key.length();
	for (i = 0; i < 256; i++)
	{
		T[i] = key[j];
		j++;
		if (j == len) j = 0;
	}
}

RC4::~RC4()
{
}

int RC4::RC4_swap()
{
	int j = 0;
	for (int i = 0; i < 256; i++)
	{
		j = (j + S[i] + T[i]) % 256;
		swap(S[i], S[j]);
	}
	return 0;
}

int RC4::out()
{
	int i = 0;
	int j = 0;
	while (true)
	{
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		swap(S[i], S[j]);
		int t = (S[i] + S[j]) % 256;
		k = S[t];
		if (getchar()) cout << k;
	}
	return 0;
}

int main()
{
	RC4 a = RC4();
	a.RC4_swap();
	a.out();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
