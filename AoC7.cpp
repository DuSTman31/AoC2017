// AoC7.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lib.h"
#include <vector>
using std::vector;

#include <iostream>
using std::cout;

#include <algorithm>
#include<map>
using std::map;

class program{
public:
	void op(){
		cout << "name " << name << " weight " << weight;
		for (unsigned int i = 0;i!=subs.size() ; i++)
		{
			cout << " sub " << subs[i];
		}
		cout << "\n";
	}
	void op(map<string, int> &a){
		cout << "name " << name << " weight " << weight;
		cout << " branch weight " << branchWeight;
		for (unsigned int i = 0; i != subs.size(); i++)
		{
			cout << " sub " << subs[i] << "(" << a[subs[i]] <<")";
		}
		cout << "\n";
	}
	void regBelows(vector<string> &b)
	{
		for (unsigned int i = 0; i != subs.size(); i++)
		{
			b.push_back(subs[i]);
		}
	}
	bool checkWeights(map<string, int> &weightMap)
	{
		bool found = false;
		int sWeight = 0;
		for (auto sIter = subs.begin() ; sIter!=subs.end() ; sIter++)
		{
			if (!found)
			{
				found = true;
				sWeight = weightMap[*sIter];
			}
			else{
				if (weightMap[*sIter] != sWeight)
				{
					return false;
				}
			
			}
		}
		return true;
	}

	string name;
	int weight = 0;
	int branchWeight = 0;
	bool branchWeightSet = false;
	vector<string> subs;

	int below = false;
};


program parseLine(string &a)
{
	program np;
	string name;
	int weight = 0;

	unsigned int i = 0;
	while (a[i] >= 'a' && a[i] <= 'z'){
		name.append(1, a[i]);
		i++;
	}
	np.name = name;

	while (a[i] == ' ' || a[i] == '\t'){
		i++;
	}

	if (a[i] == '(')
	{
		i++;
	}

	while (a[i] >= '0' && a[i] <= '9')
	{
		weight *= 10;
		weight += a[i] - '0';
		i++;
	}
	np.weight = weight;

	if (a[i] == ')')
	{
		i++;
	}

	if (i == a.length())
	{
		return np;
	}

	while (a[i] == ' ' || a[i] == '\t'){
		i++;
	}

	if (a[i] == '-' && a[i + 1] == '>')
	{
		i += 2;
	}

	while (a[i] == ' ' || a[i] == '\t'){
		i++;
	}

	vector<string> sub;
	string buf;
	while (i < a.length())
	{
		char b = a[i];
		if (b >= 'a' && b <= 'z')
		{
			buf.append(1, b);
			i++;
		}
		else
		{
			if (buf.length() > 0)
			{
				sub.push_back(buf);
				buf = string();
			}
			i++;
		}
	}
	if (buf.length() > 0)
	{
		sub.push_back(buf);
		buf = string();
		
	}

	np.subs = sub;
	return np;
}

vector<program> readPrograms(vector<string> &lines)
{
	vector<program> coll;

	for (auto lIter = lines.begin() ; lIter!= lines.end() ; lIter++)
	{
		coll.push_back(parseLine(*lIter));
	}

	return coll;
}

map<string, int> createWeightMap(vector<program> &progs)
{
	map<string, int> mp;
	for (auto pIter = progs.begin() ; pIter!=progs.end() ; pIter++)
	{
		mp[(*pIter).name] = (*pIter).weight;
	}
	return mp;
}

program getProg(vector<program> &pl, string &name)
{
	for (unsigned int i = 0; i!=pl.size() ; i++)
	{
		if (pl[i].name == name){
			return pl[i];
		}
	}
	return program();
}

int calcBranchWeight(string &a,  vector<program> &programs)
{
	program b = getProg(programs, a);
	int tw = 0;
	tw += b.weight;

	for (auto sIter = b.subs.begin(); sIter!=b.subs.end() ; sIter++)
	{
		tw += calcBranchWeight(*sIter,  programs);
	}
	return tw;
}

void traceUnbalanced(string &a, map<string, bool> &balanceMap, map<string, int> &weightMap, vector<program> &programs)
{
	program b = getProg(programs, a);
	bool unbalancedChildFound = false;

	for (auto sIter = b.subs.begin(); sIter != b.subs.end(); sIter++)
	{
		if (balanceMap[(*sIter)] == false)
		{
			cout << "Unbalance trace : " << a << " Calling sub " << *sIter <<  "\n";
			traceUnbalanced(*sIter, balanceMap, weightMap, programs);
			unbalancedChildFound = true;
		}
	}

	if (!unbalancedChildFound)
	{
		cout << "All children balanced: " << a << "\n";
		b.op(weightMap);
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		vector<string> lines = readFileAsLines("in_7.txt");
		vector<program> ps = readPrograms(lines);

		vector<string> belows;
		for (auto iIt = ps.begin(); iIt != ps.end(); iIt++)
		{
			(*iIt).regBelows(belows);
		}
		
		std::sort(belows.begin(), belows.end());
		for (auto iIt = belows.begin(); iIt != belows.end(); iIt++)
		{
			for (auto bIt = ps.begin(); bIt!=ps.end(); bIt++)
			{
				if (bIt->name == *iIt)
				{
				(*bIt).below = true;
				}
			}
		}

		string sol;
		for (auto iIt = ps.begin() ; iIt != ps.end() ; iIt++)
		{
			if (!(*iIt).below)
			{
				(*iIt).op();
				sol = (*iIt).name;
			}
		}


		map<string, int> weightMap;
		for (auto pIt = ps.begin() ; pIt!=ps.end() ; pIt++)
		{
			int bWeight = calcBranchWeight((*pIt).name, ps);
			(*pIt).branchWeight = bWeight;
			weightMap[(*pIt).name] = bWeight;
			(*pIt).branchWeightSet = true;
		}
		
		map<string, bool> balanceMap;
		for (auto pIt = ps.begin(); pIt != ps.end(); pIt++)
		{
			balanceMap[(*pIt).name] = (*pIt).checkWeights(weightMap);
		}

		traceUnbalanced(sol, balanceMap, weightMap, ps);
	}
	catch (FileReadException &e)
	{
	}

	return 0;
}

