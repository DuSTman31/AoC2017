// AoC17.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
using std::vector;
#include <iostream>
using std::cout;

class circleBuffer
{
public:
	circleBuffer()
	{
		buf.reserve(50000001);
		buf.push_back(0);
		position = 0;
	}
	void advance(unsigned int toAdv, int val)
	{
		unsigned int newPosition = position + toAdv;
		newPosition %= buf.size();

		newPosition++;
		
		auto bIter = buf.begin();
		bIter += newPosition;
		buf.insert(bIter, val);

		position = newPosition;


	}
	void out()
	{
		for (auto &b : buf)
		{
			cout << b << " ";
		}
		cout << "\n";
	}
private:
	vector<int> buf;
	unsigned int position;

};

class circleBufferN
{
public:
	circleBufferN()
	{
		buf.reserve(2);
		buf.push_back(0);
		buf.push_back(0);

		position = 0;
		size = 1;
	}
	void advance(unsigned int toAdv, int val)
	{
		unsigned int newPosition = position + toAdv;
		newPosition %= size;

		newPosition++;

		if (newPosition == 1)
		{
			buf[1] = val;
		}

		position = newPosition;
		size++;

	}
	void out()
	{
		for (auto &b : buf)
		{
			cout << b << " ";
		}
		cout << "\n";
	}
private:
	vector<int> buf;
	unsigned int position;
	unsigned int size;

};

int _tmain(int argc, _TCHAR* argv[])
{
	circleBufferN a;

	for (unsigned int i = 1; i!=50000001; i++)
	{
		a.advance(363, i);

	}
	a.out();

	return 0;
}

