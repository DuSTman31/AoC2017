// AoC9.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lib.h"
#include <iostream>
using std::cout;
#include <algorithm>
using std::for_each;

class formatException{};

struct group{
	group(){};
	group(const group &a){
		for (auto sgIter = a.subGroups.begin(); sgIter!=a.subGroups.end() ; sgIter++)
		{
			group *ng = new group(**sgIter);
			subGroups.push_back(ng);
		}
	}
	~group()
	{
		for_each(subGroups.begin(), subGroups.end(), [](group *subGroup){ delete subGroup; });
	}

	void enumerate(int score_)
	{
		score = score_;
		for_each(subGroups.begin(), subGroups.end(), [&score_](group *subGroup){ subGroup->enumerate(score_ + 1); });
	}

	unsigned int tallyScore()
	{
		unsigned int tot = 0;
		tot += score;
		for_each(subGroups.begin(), subGroups.end(), [&tot](group *subGroup){ tot += subGroup->tallyScore();  });

		return tot;
	}

	int score = 0;
	vector<group*> subGroups;
};


unsigned int scanGarbage(string &buf, unsigned int offset, unsigned int &garbageChars)
{
	unsigned int i = 0;
	if (buf[offset] != '<')
	{
		return 0;
	}

	i++; 
	while (offset + i != buf.length())
	{
		if (buf[offset + i] == '!'){
			i+=2;
		}
		else if (buf[offset + i] == '>')
		{
			i++;
			return i;
		}
		else
		{
			i++;
			garbageChars++;
		}

	}
	return 0;
}

group scanGroup(string &in, unsigned int offset, unsigned int *consumed, unsigned int &garbageChars)
{
	group ng;
	unsigned int i = 0;
	if (in[offset + i] != '{')
	{
		throw formatException();
	}

	i++;

	while (true)
	{
		if (in[offset + i] == '<')
		{
			unsigned int gc = scanGarbage(in, offset + i, garbageChars);
			i += gc;
		}
		else if (in[offset + i] == '{')
		{
			unsigned int gLen = 0;
			group sg = scanGroup(in, offset + i, &gLen, garbageChars);
			group *nsg = new group(sg);
			i += gLen;
			ng.subGroups.push_back(nsg);
		}

		if (in[offset + i] == '}'){
			*consumed = i + 1;
			break;
		}
		else if (in[offset + i] != ',')
		{
			char c = in[offset + i];
			throw formatException();
		}
		i++;
	}

	return ng;
}

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		string input = readWholeFile("in_9.txt");

		unsigned int bc = 0;
		unsigned int garbageChars = 0;
		group sg = scanGroup(input, 0, &bc, garbageChars);

		sg.enumerate(1);
		

		unsigned int totalScore = 0;
		totalScore = sg.tallyScore();
		cout << "Total score " << totalScore << "\n";

		unsigned int totalChars = 0;
		cout << "Total chars " << garbageChars << "\n";

	}
	catch (FileReadException &e)
	{
	}
	return 0;
}

