// RSA.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <math.h>
#include <stdlib.h>
#define SIZE 10000
#define ULL unsigned long long

using namespace std;

ULL S[SIZE];
char T[SIZE];

ULL gcd(ULL a, ULL b)
{
	return b > 0 ? gcd(b, a % b) : a;
}

ULL random_prime(ULL size)
{
	string key;
	ULL k = 0;

	ULL i, j;

	memset(S, size, sizeof(S));
	memset(T, size, sizeof(T));
	for (i = 0; i < size; i++)
	{
		S[i] = i;
	}
	cout << "Please enter the key：" << endl;
	cin >> key;
	j = 0;
	ULL len = key.length();
	for (i = 0; i < size; i++)
	{
		T[i] = key[j];
		cout << T[i];
		j++;
		if (j == len) j = 0;
	}
	cout << endl;

	cout << "Please waiting for swap" << endl;
	//swap
	j = 0;
	for (i = 0; i < size; i++)
	{
		j = (j + S[i] + T[i]) % size;
		swap(S[i], S[j]);
	}
	cout << "swap finished" << endl;



	ULL count = 0;
	while (1)
	{
		i = 0;
		cout << "enter the while" << endl;

		//new random number//随机一个大数
		i = 0;
		j = 0;
		i = (i + 1) % size;
		j = (j + S[i]) % size;
		swap(S[i], S[j]);
		ULL t = (S[i] + S[j]) % size;
		k = S[t];
		cout << k << endl;

		while (i < size)
		{
			if ((S[i] < k) && (S[i] > 1))
			{
				if ((k % 2 == 0) || (k % 3 == 0) || (k % 5 == 0) || (k % 7 == 0) || (k % 11 == 0) || (k % 13 == 0) || (k % 17 == 0) || (k % 19 == 0) || k % S[i] == 0)
				{
					cout << "Unluckly! " << k << " is not a prime." << endl;
					break;//若找到因数 则重新产生一个大数
				}
				count++;
				cout << count << endl;
			}
			if (count == 1000) break;
			i++;
		}
		if (count == 1000) break;
	}
	return k;
}

ULL E(ULL n, ULL e, ULL *M, ULL len, ULL *C)
{
	for (ULL i = 0; i < len; i++)
	{
		C[i] = M[i];
		for (ULL j = 0; j < e - 1;j++)//C[i] = (ULL)pow(M[i], d) % n;
		{
			C[i] = C[i] * M[i] % n;
		}
	}
	for (ULL i = 0; i < len; i++)
	{
		cout << C[i] << " ";
	}
	cout << endl;
	return 0;
}

ULL D(ULL n, ULL d, ULL *C, ULL len, ULL *M)
{
	for (ULL i = 0; i < len; i++)
	{
		M[i] = C[i];
		for (ULL j = 0; j < d - 1; j++)//M[i] = (ULL)pow(C[i], d) % n;
		{
			M[i] = M[i]* C[i] % n;
		}
	}
	for (ULL i = 0; i < len; i++)
	{
		cout << M[i] << " ";
	}
	cout << endl;
	return 0;
}

int main()
{
	ULL p = random_prime(SIZE);
	ULL q = random_prime(SIZE);
	ULL n = p * q;
	ULL x = (p - 1) * (q - 1);
	cout << x << endl;
	ULL e;
	ULL d;
	cout << "The number you can choose for e:" << endl;
	for (e = x - 1; e > x-100; e--)
	{
		if (gcd(e, x) == 1)
		{
			cout << e << " ";
		}
	}
	cout << endl << "Please choose the e:" << endl;
	cin >> e;

	for (d = x - 1; d > 0; d--)
	{
		if ((e * d) % x == 1)
		{
			break;
		}
	}

	ULL M[64] = { 3462,34543,2345,234,6788 };
	ULL C[64] = { 0 };
	ULL R[64] = { 0 };
	E(n, e, M, 5, C);
	D(n, d, C, 5, R);
	
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
