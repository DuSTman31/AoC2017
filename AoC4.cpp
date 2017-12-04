// AoC4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdio>
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <algorithm>
using std::sort;

vector<string> split(char *in, unsigned int size)
{
	string buf;
	vector<string> lines;

	for (unsigned int i = 0 ; i!=size ; i++)
	{
		if (in[i] == '\n')
		{
			lines.push_back(buf);
			buf = string();
		}
		else if (in[i] == '\r')
		{
		}
		else
		{
			buf.append(1, in[i]);
		}
	}
	if (buf.length() > 0) {
		lines.push_back(buf);
	}

	return lines;
}

vector<string> splitWords(string &a)
{
	string buf;
	vector<string> words;

	for (unsigned int i = 0; i!=a.size() ; i++) {
		if (a[i] == ' ' || a[i] == '\t'){
			words.push_back(buf);
			buf = string();
		}
		else
		{
			buf.append(1, a[i]);
		}
	}

	if (buf.length() > 0) {
		words.push_back(buf);
	}
	return words;
}

bool valid(vector<string> &a)
{
	std::sort(a.begin(), a.end());

	bool val = true;

	for (unsigned int i = 0; i + 1 != a.size(); i++)
	{
		if (a[i] == a[i + 1])
		{
			return false;
		}
	}

}

bool valid2(vector<string> &a)
{
	vector<string> sList;
	for (auto aIter = a.begin() ; aIter!=a.end() ; aIter++)
	{
		string b = *aIter;
		sort(b.begin(), b.end());
		sList.push_back(b);
	}

	std::sort(sList.begin(), sList.end());

	bool val = true;

	for (unsigned int i = 0; i + 1 != sList.size(); i++)
	{
		if (sList[i] == sList[i + 1])
		{
			return false;
		}
	}

}

int noValid(vector<vector<string>> &a)
{
	int val = 0;

	for (auto iIter = a.begin() ; iIter!=a.end() ; iIter++)
	{
		if (valid(*iIter))
		{
			val++;
		}
	}

	return val;
}

int noValid2(vector<vector<string>> &a)
{
	int val = 0;

	for (auto iIter = a.begin(); iIter != a.end(); iIter++)
	{
		if (valid2(*iIter))
		{
			val++;
		}
	}

	return val;
}

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int fileSize = 0;
	FILE *fh = fopen("in.txt", "rb");
	fseek(fh, 0, SEEK_END);
	fileSize = ftell(fh);
	fseek(fh, 0, SEEK_SET);
	char *buf = new char[fileSize];
	fread(buf, 1, fileSize, fh);

	vector<string> lines = split(buf, fileSize);

	vector<vector<string>> wordListList;

	for (auto iIter = lines.begin() ; iIter!=lines.end() ; iIter++)
	{
		wordListList.push_back(splitWords(*iIter));
	}

	int v = 0;
	int vv = 0;
	v  = noValid(wordListList);
	vv = noValid2(wordListList);

	printf("No valid (part 1): %u \n", v);
	printf("No valid (part 2): %u \n", vv);


	return 0;
}

