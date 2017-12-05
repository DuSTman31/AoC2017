
#include "lib.h"

#include <vector>
using std::vector;
#include <iostream>
using std::cout;

void part1(vector<int> in)
{
	int position = 0;
	int steps = 0;

	while (position >= 0 && position < in.size()) {
		int toJump = in[position];
		in[position]++;
		position += toJump;
		steps++;
	}
	cout << "Part 1 steps: " << steps << "\n";
}


void part2(vector<int> in)
{
	int position = 0;
	int steps = 0;

	while (position >= 0 && position < in.size()) {
		int toJump = in[position];
		if (toJump >= 3)
		{
			in[position]--;
		}
		else
		{
			in[position]++;
		}

		position += toJump;
		steps++;
	}
	cout << "Part 2 steps: " << steps << "\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		vector<string> results = readFileAsLines("in_5.txt");

		vector<int> res_i;

		for (unsigned int i = 0 ; i!=results.size() ; i++) {
			res_i.push_back(stoi(results[i]));
		}

		part1(res_i);
		part2(res_i);

	}
	catch (FileReadException &e)
	{
	}
	return 0;
}

