#include "stdafx.h"
#include "Lib.h"
#include <iostream>
using std::cout;
using std::cerr;
#include <map>
using std::map;

struct line{
	void move()
	{
		if (direction == 0)
		{
			if (position == severity - 1)
			{
				direction = 1;
				position--;
			}
			else
			{
				position++;
			}
		}
		else
		{
			if (position == 0)
			{
				direction = 0;
				position++;
			}
			else
			{
				position--;
			}
		}
	}

	void move(int toMove)
	{
		int period = (severity - 1) * 2;
		toMove %= period;

		for (int i = 0; i != toMove; i++)
		{
			move();
		}
	}

	void reset()
	{
		direction = 0;
		position = 0;
	}

	int number = 0;
	int severity = 0;

	int position = 0;
	int direction = 0;
};

line parseLine(const string &in)
{
	line lb;
	vector<string> splitInput = split(in);

	lb.number = stoi(trimTrailingColons(splitInput[0]));
	lb.severity = stoi(splitInput[1]);
	return lb;
}

vector<line> parseLines(const vector<string> &input)
{
	vector<line> op;
	for (auto l : input)
	{
		op.push_back(parseLine(l));
	}
	return op;
}

void progressScanners(vector<line> &a)
{
	for (auto &l : a)
	{
		l.move();
	}
}

int isCaught(vector<line> &lines, map<int, line*> &lm, int greatestNumber, bool &caught)
{
	int caughtSeverity = 0;
	for (int i = 0; i != greatestNumber + 1; i++)
	{

		if (lm.count(i) != 0)
		{
			if (lm[i]->position == 0)
			{
				caughtSeverity += (lm[i]->severity * i);
				caught = true;
			}
		}

		progressScanners(lines);

	}
	return caughtSeverity;
}

int main(int argc, char* argv[])
{
	try
	{
		vector<string> input = readFileAsLines("in_13.txt");

		vector<line> parsedInput = parseLines(input);

		int greatestNumber = 0;
		for (auto &l : parsedInput)
		{
			if (l.number > greatestNumber)
			{
				greatestNumber = l.number;
			}
		}

		map<int, line*> lm;
		for (auto &l : parsedInput)
		{
			lm[l.number] = &l;
		}

		int caughtSeverity = 0;
		bool cc = false;
		caughtSeverity = isCaught(parsedInput, lm, greatestNumber, cc);
		cout << "Part 1 severity " << caughtSeverity << "\n";

		int j = 0;
		int delay = 0;
		while (true)
		{
			for (auto &l : parsedInput)
			{
				l.reset();
			}


			for (auto &l : parsedInput)
			{
				l.move(delay);
			}



			//for (int i = 0;i!=delay ; i++)
			//{
			//	progressScanners(parsedInput);
			//}

			bool caught = false;
			isCaught(parsedInput, lm, greatestNumber, caught);
			if (!caught)
			{
				cout << "Not caught at " << delay << " = " << caught <<"\n";
				break;
			}
			else
			{
				delay += 1;
				//cout << "Caught at " << delay << " = " << caught << "\n";
			}
			
		}

	}
	catch (FileReadException &e)
	{
	}
	return 0;
}

