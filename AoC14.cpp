// AoC14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "Lib.h"
#include <iostream>
using std::cout;
#include <algorithm>
using std::for_each;
using std::sort;
using std::unique;

class knot{
public:
	knot()
	{
		for (int i = 0; i != 256; i++)
		{
			list.push_back(i);
		}
	}

	void doPass(int length)
	{
		unsigned int startPosition = currentPosition;
		unsigned int endPosition = startPosition + length - 1;

		while (startPosition < endPosition)
		{
			int temp = list[startPosition%list.size()];
			list[startPosition%list.size()] = list[endPosition%list.size()];
			list[endPosition%list.size()] = temp;
			endPosition--;
			startPosition++;
		}

		currentPosition += length;
		currentPosition += skip++;
		currentPosition %= list.size();
	}

	void op()
	{
		for (auto i : list)
		{
			cout << i << " ";
		}
		cout << "\n";
	}

	string denseHash()
	{
		string dh;
		int block = 0;
		int inBlock = 0;
		for (unsigned int i = 0; i != list.size(); i++)
		{
			block ^= list[i];
			inBlock++;
			if (inBlock == 16)
			{
				char pallette[] = "0123456789abcdef";
				char high = (block & 0xF0) >> 4;
				char low = block & 0x0F;

				dh.append(1, pallette[high]);
				dh.append(1, pallette[low]);
				block = 0;
				inBlock = 0;
			}
		}
		return dh;
	}
	void round(vector<int> &lengths)
	{
		for (auto i : lengths)
		{
			doPass(i);
		}
	}
	void doHash(vector<int> &lengths)
	{
		vector<int> data = lengths;
		data.push_back(17);
		data.push_back(31);
		data.push_back(73);
		data.push_back(47);
		data.push_back(23);
		for (unsigned int i = 0; i!=64 ; i++)
		{
			round(data);
		}
	}
private:

	unsigned int currentPosition = 0;
	unsigned int skip = 0;
	vector<int> list;
};

string itos(int in)
{
	string backwardsString;

	if (in == 0)
	{
		return string("0");
	}

	int in_ = in;
	while (in_ > 0)
	{
		backwardsString.append(1, (in_ % 10) + '0');
		in_ /= 10;
	}
	
	string forwardsString;
	for (auto sIter = backwardsString.rbegin(); sIter!=backwardsString.rend() ; sIter++)
	{
		forwardsString.append(1, *sIter);
	}
	return forwardsString;
}

int ctoi(char in)
{
	if (in >= '0' && in <= '9')
	{
		return (in - '0');
	}
	else if (in >= 'a' && in <= 'f')
	{
		return (in - 'a' + 10);
	}
	return 0;
}

struct state
{

	int mark = 0;
	bool hasBlock;
};

void mark(int x, int y, vector<vector<state>> &sv, int m)
{
	if (sv[y][x].mark == m)
	{
		return;
	}
	else
	{
		sv[y][x].mark = m;

		if (x+1 < sv[y].size() && sv[y][x+1].hasBlock)
		{
			mark(x + 1, y, sv, m);
		}
		if (x != 0 && sv[y][x - 1].hasBlock)
		{
			mark(x - 1, y, sv, m);
		}
		if (y + 1 < sv.size() && sv[y + 1][x].hasBlock)
		{
			mark(x, y + 1, sv, m);
		}
		if (y != 0 && sv[y - 1][x].hasBlock)
		{
			mark(x, y - 1, sv, m);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	string data = "hwlqcszp";
	//string data = "flqrgnkx";
	unsigned int totalUsed = 0;
	vector<vector<state>> svv;
	for (unsigned int i = 0; i != 128; i++)
	{
		knot k;
		string md = data;
		md.append(1, '-');
		md.append(itos(i));
		
		cout << md << "\n";

		vector<int> processedData;
		for (char j : md)
		{
			processedData.push_back(j);
		}

		k.doHash(processedData);

		//totalUsed = 0;
		string result = k.denseHash();
		cout << "Result " << result << "\n";
		vector<state> sv;
		for (auto i : result)
		{
			int b = ctoi(i);

			for (unsigned int j = 0; j!=4; j++)
			{
				int o = (b & 0x08);
				if (o == 8)
				{
					state p;
					p.hasBlock = true;
					sv.push_back(p);
				}
				else
				{
					totalUsed++;
					state p;
					p.hasBlock = false;
					sv.push_back(p);
				}
				b &= 0x07;
				b <<= 1;
			}
		}
		svv.push_back(sv);
	}
	cout << "No blocks used " << totalUsed << "\n";

	int m = 0;
	for (int y = 0; y != svv.size(); y++)
	{
		for (int x = 0; x!=svv[y].size() ; x++)
		{
			if (svv[y][x].hasBlock)
			{
				mark(x, y, svv, m++);
			}
		}
	}

	vector<int> marksSeen;
	for (int y = 0; y != svv.size(); y++)
	{
		for (int x = 0; x != svv[y].size(); x++)
		{
			if (svv[y][x].hasBlock)
			{
				marksSeen.push_back(svv[y][x].mark);
			}
		}
	}

	unsigned int noMarks = 0;
	sort(marksSeen.begin(), marksSeen.end());
	auto umEndIter = unique(marksSeen.begin(), marksSeen.end());
	for_each(marksSeen.begin(), umEndIter, [&noMarks](int b){noMarks++; });

	cout << "Pt2 marks " << noMarks << "\n";

	return 0;
}

