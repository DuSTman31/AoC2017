// AoC15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdint>
#include <iostream>
using std::cout;

class generator
{
public:
	generator(uint64_t l, uint64_t m, int r) : last(l), mult(m), remn(r) {};

	uint64_t gen()
	{
		while (true)
		{
			uint64_t temp = last;
			temp *= mult;
			temp %= 2147483647;
			last = temp;
			if (temp % remn == 0)
			{
				return temp;
			}
		}
	}
private:
	uint64_t last;
	uint64_t mult;
	int remn;
};


class judge
{
public:
	void round(uint64_t a, uint64_t b)
	{
		if ((a & 0x000000000000FFFF) == (b & 0x000000000000FFFF))
		{
			total++;
		}
	}
	unsigned int getTotal()
	{
		return total;
	}
private:
	unsigned int total = 0;
};

int _tmain(int argc, _TCHAR* argv[])
{
	generator a(679, 16807, 1);
	generator b(771, 48271, 1);
	judge j;

	for (int i = 0; i != 40000000; i++)
	{
		j.round(a.gen(), b.gen());
	}
	cout << "Part 1 Total " << j.getTotal() << "\n";

	generator a2(679, 16807, 4);
	generator b2(771, 48271, 8);
	judge j2;
	for (int i = 0; i != 5000000; i++)
	{
		j2.round(a2.gen(), b2.gen());
	}
	cout << "Total " << j2.getTotal() << "\n";

	return 0;
}

