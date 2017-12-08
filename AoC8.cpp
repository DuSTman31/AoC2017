// AoC8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lib.h"

#include <iostream>
using std::cout;
#include <algorithm>
#include <map>
using std::map;

enum class op{Inc, Dec};

enum class conditions{
	Less, LessOrEq, Eq, NotEq, Greater, GreaterOrEq
};

class condition{

public:
	string regName;
	int value = 0;
	conditions a;
};

class instruction{
public:
	op oper;
	string regName;
	int value = 0;
	condition cond;
};

condition parseCondition(const string &a, unsigned int offset)
{
	condition nc;
	unsigned int i = offset;
	if (matchString(string("if"), a, i))
	{
		i += 2;
	}

	i += consumeWhiteSpace(a, i);

	string buf;
	while (a[i] >= 'a' && a[i] <= 'z')
	{
		buf.append(1, a[i]);
		i++;
	}
	nc.regName = buf;

	i += consumeWhiteSpace(a, i);

	string less("<");
	string lessOrEq("<=");
	string eq("==");
	string notEq("!=");
	string greater(">");
	string greaterEqual(">=");
	if (matchString(lessOrEq, a, i)){
		nc.a = conditions::LessOrEq;
		i += lessOrEq.length();
	}
	else if (matchString(less, a, i)){
		nc.a = conditions::Less;
		i += less.length();
	}
	else if (matchString(eq, a, i)){
		nc.a = conditions::Eq;
		i += eq.length();
	}
	else if (matchString(notEq, a, i)){
		nc.a = conditions::NotEq;
		i += notEq.length();
	}
	else if (matchString(greaterEqual, a, i)){
		nc.a = conditions::GreaterOrEq;
		i += greaterEqual.length();
	}
	else if (matchString(greater, a, i)){
		nc.a = conditions::Greater;
		i += greater.length();
	}


	i += consumeWhiteSpace(a, i);

	unsigned int bytesConsumed = 0;
	nc.value += decodeInt(a, i, &bytesConsumed);

	return nc;
}

instruction parseLine(string &a)
{
	instruction ni;
	string buf;

	unsigned int i = 0;
	while (a[i] >= 'a' && a[i] <= 'z')
	{
		buf.append(1, a[i]);
		i++;
	}
	ni.regName = buf;
	buf = string();

	i += consumeWhiteSpace(a, i);

	string inc("inc");
	string dec("dec"); 

	if (matchString(inc, a, i)){
		ni.oper = op::Inc;
		i += 3;
	}
	else if (matchString(dec, a, i))
	{
		ni.oper = op::Dec;
		i += 3;
	}

	i += consumeWhiteSpace(a, i);

	unsigned int bytesConsumed = 0;
	ni.value += decodeInt(a, i, &bytesConsumed);
	i += bytesConsumed;

	i += consumeWhiteSpace(a, i);

	ni.cond = parseCondition(a, i);

	return ni;
}

vector<string> getRegisters(vector<instruction> &insns)
{
	vector<string> regs;
	for (unsigned int i = 0 ; i!=insns.size() ; i++)
	{
		regs.push_back(insns[i].regName);
		regs.push_back(insns[i].cond.regName);
	}
		
	std::sort(regs.begin(), regs.end());

	vector<string> reg2;
	string prevReg;
	bool prFound = false;

	for (unsigned int i = 0; i != regs.size(); i++)
	{
		if (!prFound)
		{
			prFound = true;
			prevReg = regs[i];
		}
		else
		{
			if (regs[i] != prevReg)
			{
				reg2.push_back(regs[i]);
				prevReg = regs[i];
			}
		}

	}

	return reg2;
}

bool evalCondition(map<string, int> &regState, condition &cond)
{
	if (cond.a == conditions::Less){
		if (regState[cond.regName] < cond.value)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	else if (cond.a == conditions::LessOrEq){
		if (regState[cond.regName] <= cond.value)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(cond.a == conditions::Eq){
		if (regState[cond.regName] == cond.value)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	else if(cond.a == conditions::NotEq){
		if (regState[cond.regName] != cond.value)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	else if (cond.a == conditions::Greater){
		if (regState[cond.regName] > cond.value)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	else if (cond.a == conditions::GreaterOrEq){
		if (regState[cond.regName] >= cond.value)
		{
			return true;
		}
		else
		{
			return false;
		}

	}

}

void doInstruction(map<string, int> &regValues, instruction &ins, int *highestVal)
{
	if (evalCondition(regValues, ins.cond))
	{
		if (ins.oper == op::Dec)
		{
			int val = regValues[ins.regName];
			val -= ins.value;
			regValues[ins.regName] = val;

			if (val > *highestVal)
			{
				*highestVal = val;
			}
		}
		else if (ins.oper == op::Inc)
		{
			int val = regValues[ins.regName];
			val += ins.value;
			regValues[ins.regName] = val;

			if (val > *highestVal)
			{
				*highestVal = val;
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		vector<string> results = readFileAsLines("in_8.txt");

		vector<instruction> insnList;
		for (int i = 0; i!=results.size() ; i++)
		{
			insnList.push_back(parseLine(results[i]));
		}

		vector<string> registers;
		registers = getRegisters(insnList);

		map<string, int> registerValues;
		for (int i = 0 ; i!=registers.size() ; i++)
		{
			registerValues[registers[i]] = 0;
		}

		int highestVal = 0;
		for (unsigned int i = 0 ; i!= insnList.size() ; i++)
		{
			doInstruction(registerValues, insnList[i], &highestVal);
		}

		int greatest = 0;
		bool greatestFound = false;
		for (auto rIter = registerValues.begin(); rIter!=registerValues.end() ; rIter++)
		{
			if (!greatestFound)
			{
				greatestFound = true;
				greatest = (*rIter).second;
			}
			else if (greatest < (*rIter).second)
			{
				greatest = (*rIter).second;
			}
		}

		cout << "greatest " << greatest << "\n";
		cout << "highest val " << highestVal << "\n";
	}
	catch (FileReadException &e)
	{
	}
	return 0;
}

