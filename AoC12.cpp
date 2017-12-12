
#include "Lib.h"
#include <iostream>
using std::cerr;
using std::cout;
#include <map>
using std::map;
#include <algorithm>
using std::sort;

struct program{
	int number;
	int Mark = 0;
	vector<int> connections;
};

program parseProgram(const string &in)
{
	program np;
	vector<string> words = split(in);
	np.number = stoi(words[0]);

	for (unsigned int i = 2; i!=words.size() ; i++)
	{
		string is = trimTrailingCommas(words[i]);
		np.connections.push_back(stoi(is));
	}
	return np;
}

vector<program> parsePrograms(vector<string> &in)
{
	vector<program> programs;
	for (auto line : in)
	{
		programs.push_back(parseProgram(line));
	}
	return programs;
}

void MarkPass(map<int, program> &pm, unsigned int mark, int start, int &visited)
{
	if (pm.count(start) != 0)
	{
		int mm = pm[start].Mark;
		if (mm == mark){
			return;
		}
		else
		{
			visited++;
			pm[start].Mark = mark;

			for (auto p : pm[start].connections)
			{
				MarkPass(pm, mark, p, visited);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	try{
		vector<string> input = readFileAsLines("in_12.txt");
	
		vector<program> programs = parsePrograms(input);

		map<int, program> progMap;
		for (auto p : programs)
		{
			progMap[p.number] = p;
		}

		int visited = 0;
		MarkPass(progMap, 10278, 0, visited);
		cout << "Part 1 " << visited << "\n";

		int groups = 0;
		int mark = 11;
		visited = 0;
		for (auto p : programs)
		{
			MarkPass(progMap, mark++, p.number, visited);
		}
		
		vector<int> noSeen;
		for (auto &n : progMap)
		{
			noSeen.push_back(n.second.Mark);
		}
		sort(noSeen.begin(), noSeen.end());

		bool found = false;
		int lastMark = 0;
		int noNetworks = 0;
		for (unsigned int i = 0; i != noSeen.size(); i++)
		{
			if (!found)
			{
				found = true;
				lastMark = noSeen[i];
				noNetworks++;
			}
			else if (lastMark != noSeen[i])
			{
				lastMark = noSeen[i];
				noNetworks++;
			}
		}

		cout << "Part 2 - No networks " << noNetworks << "\n";
	}
	catch (FileReadException &e)
	{
		cerr << "Could not read file.\n";
	}
	return 0;
}

