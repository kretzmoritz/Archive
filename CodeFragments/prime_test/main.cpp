#include <iostream>

class BitArray
{
public:
	BitArray(int _bytes)
	{
		p = new char[_bytes](); //zero-mem
	}

	~BitArray()
	{
		delete[] p;
		p = NULL;
	}
	
	void setBit(int _n)
	{
		int i = _n / 8;
		int shift = _n - (i * 8);
		p[i] |= 1 << shift;
	}

	bool operator[](int _n)
	{
		int i = _n / 8;
		int shift = _n - (i * 8);
		return (p[i] >> shift) & 1;
	}

private:
	char* p;
};

// Sieve of Eratosthenes
// http://en.wikipedia.org/wiki/Primality_test

int main()
{
	int n = 0;

	while (n < 2)
	{
		std::cin >> n;
	}

	int bytesNeeded = (n / 8) + 1;
	BitArray a(bytesNeeded);

	for (int i = 2; i < sqrt(n); ++i)
	{
		if (!a[i])
		{
			int j = 0;
			int k = int(pow(i, 2)) + (j * i);

			while (k < n)
			{
				a.setBit(k);
				++j;
				k = int(pow(i, 2)) + (j * i);
			}
		}
	}

	int prime_count = 0;

	for (int i = 2; i < n; ++i)
	{
		if (!a[i])
		{
			std::cout << i << std::endl;
			++prime_count;
		}
	}

	std::cout << "prime_count: " << prime_count << std::endl;

	system("pause");
}