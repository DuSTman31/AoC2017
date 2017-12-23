#include "Lib.h"
#include <iostream>
using std::cerr;
using std::cout;
#include <map>
using std::map;

class FormatException{  };

enum class operation{snd, set, add, mul, mod, rcv, jgz};

struct operand{
	long long int numeric = 0;
	char reg = ' ';
	bool isReg = true;
};

struct instruction
{
	operation op;
	operand a;
	operand b; 
};

instruction parseIns(const string &in)
{
	instruction ni;
	unsigned int i = 0;
	int noOperands = 0;

	if (matchString("snd", in, 0))
	{
		ni.op = operation::snd;
		i += 3;
		noOperands = 1;
	}
	else if (matchString("set", in, 0))
	{
		ni.op = operation::set;
		i += 3;
		noOperands = 2;
	}
	else if (matchString("add", in, 0))
	{
		ni.op = operation::add;
		i += 3;
		noOperands = 2;
	}
	else if (matchString("mul", in, 0))
	{
		ni.op = operation::mul;
		i += 3;
		noOperands = 2;
	}
	else if (matchString("mod", in, 0))
	{
		ni.op = operation::mod;
		i += 3;
		noOperands = 2;
	}
	else if (matchString("rcv", in, 0))
	{
		ni.op = operation::rcv;
		i += 3;
		noOperands = 1;
	}
	else if (matchString("jgz", in, 0))
	{
		ni.op = operation::jgz;
		i += 3;
		noOperands = 2;
	}
	else
	{
		throw FormatException();
	}

	i += consumeWhiteSpace(in, i);
	if (in[i] >= 'a' && in[i] <= 'z')
	{
		ni.a.isReg = true;
		ni.a.reg = in[i];
		i++;
	}
	else
	{
		ni.a.isReg = false;
		unsigned int consume = 0;
		ni.a.numeric = decodeInt(in, i, &consume);
		i += consume;
	}

	if (noOperands == 2)
	{
		i += consumeWhiteSpace(in, i);
		if (in[i] >= 'a' && in[i] <= 'z')
		{
			ni.b.isReg = true;
			ni.b.reg = in[i];
			i++; 
		}
		else
		{
			ni.b.isReg = false;
			unsigned int consume = 0;
			ni.b.numeric = decodeInt(in, i, &consume);
			i += consume;
		}

	}
	return ni;
}

vector<instruction> parseProgram(vector<string> &lines)
{
	vector<instruction> prog;
	for (auto &l : lines)
	{
		prog.push_back(parseIns(l)); 
	}
	return prog;
}

class inputQueue
{
public:
	void push(unsigned long long c)
	{
		queue.push_back(c);
	}
	unsigned long long pop()
	{
		unsigned long long c = queue[0];
		queue.erase(queue.begin());
		return c;
	}
	bool hasData()
	{
		if (queue.size() > 0)
		{
			return true;
		}
		return false;
	}
private:
	vector<unsigned long long> queue;

};



class machine
{
public:
	enum class state{running, awaitInput, stopped};

	machine(unsigned int p)
	{
		machineState = state::running;
		matchedMachine = NULL;
		freq = 0;
		pid = p;
		for (int i = 0; i!=256 ; i++)
		{
			char j = i;
			regs[j] = 0;
		}
		regs['p'] = pid;
	}
	void doInsn_1(vector<instruction> &j)
	{
		unsigned int pc = 0;


		long long int v = 0;
		while (pc < j.size())
		{
			instruction i = j[pc];
			switch (i.op)
			{
			case operation::add:
				v = regs[i.a.reg];
				if (i.b.isReg)
				{
					v += regs[i.b.reg];
				}
				else
				{
					v += i.b.numeric;
				}
				regs[i.a.reg] = v;
				pc++;
				break;
			case operation::jgz:
				v = 0;
				v = val(i.a);
				if (v > 0)
				{
					if (i.b.isReg)
					{
						pc += regs[i.b.reg];
					}
					else
					{
						pc += i.b.numeric;
					}
				}
				else
				{
					pc++;
				}
				break;
			case operation::mod:
				v = regs[i.a.reg];
				if (i.b.isReg)
				{
					v %= regs[i.b.reg];
				}
				else
				{
					v %= i.b.numeric;
				}
				regs[i.a.reg] = v;
				pc++;
				break;
			case operation::mul:
				v = regs[i.a.reg];
				if (i.b.isReg)
				{
					v *= regs[i.b.reg];
				}
				else
				{
					v *= i.b.numeric;
				}
				regs[i.a.reg] = v;
				pc++;
				break;
			case operation::rcv:
				if (i.a.isReg)
				{
					if (regs[i.a.reg] != 0)
					{
						cout << "Freq " << freq << "\n";
					}

				}
				else if (i.a.numeric != 0)
				{
					cout << "Freq " << freq << "\n";
				}
				pc++;
				break;
			case operation::set:
				if (i.b.isReg)
				{
					regs[i.a.reg] = regs[i.b.reg];
				}
				else
				{
					regs[i.a.reg] = i.b.numeric;
				}
				pc++;
				break;
			case operation::snd:
				if (i.a.isReg)
				{
					freq = regs[i.a.reg];
				}
				else
				{
					freq = i.a.numeric;
				}
				pc++;
				break;
			}

		}
	}
	void doInsn_2(vector<instruction> &j)
	{
		long long v = 0;

		if (pc >= j.size())
		{
			machineState = state::stopped;
		}

		if (machineState == state::running)
		{

			instruction i = j[pc];
			switch (i.op)
			{
			case operation::add:
				v = regs[i.a.reg];
				if (i.b.isReg)
				{
					v += regs[i.b.reg];
				}
				else
				{
					v += i.b.numeric;
				}
				regs[i.a.reg] = v;
				pc++;
				break;
			case operation::jgz:
				v = 0;
				v = val(i.a);
				if (v > 0)
				{
					if (i.b.isReg)
					{
						pc += regs[i.b.reg];
					}
					else
					{
						pc += i.b.numeric;
					}
				}
				else
				{
					pc++;
				}
				break;
			case operation::mod:
				v = regs[i.a.reg];
				if (i.b.isReg)
				{
					v %= regs[i.b.reg];
				}
				else
				{
					v %= i.b.numeric;
				}
				regs[i.a.reg] = v;
				pc++;
				break;
			case operation::mul:
				v = regs[i.a.reg];
				if (i.b.isReg)
				{
					v *= regs[i.b.reg];
				}
				else
				{
					v *= i.b.numeric;
				}
				regs[i.a.reg] = v;
				pc++;
				break;
			case operation::rcv:
				if (!iq.hasData())
				{
					machineState = state::awaitInput;
				}
				else
				{
					if (i.a.isReg)
					{
						regs[i.a.reg] = iq.pop();
					}
					pc++;
				}


				break;
			case operation::set:
				if (i.b.isReg)
				{
					regs[i.a.reg] = regs[i.b.reg];
				}
				else
				{
					regs[i.a.reg] = i.b.numeric;
				}
				pc++;
				break;
			case operation::snd:
				if (i.a.isReg)
				{
					matchedMachine->iq.push(regs[i.a.reg]);
					matchedMachine->machineState = state::running;
					sent++;
					//freq = regs[i.a.reg];
				}
				pc++;
				break;
			}
		}
	}
	long long int val(operand &o)
	{
		if (o.isReg)
		{
			return regs[o.reg];
		}
		else
		{
			return o.numeric;
		}
	}
	inputQueue iq;
	map<char, long long int> regs;
	int freq;
	machine *matchedMachine;
	state machineState;
	unsigned int sent = 0;
	unsigned int pc = 0;
	unsigned int pid = 0;
};


int main(int argc, char* argv[])
{
	try
	{
		vector<string> inLines = readFileAsLines("in_18.txt"); 

		vector<instruction> program = parseProgram(inLines);

		machine m(1);
		//m.doInsn_1(program);

		machine m1(0);
		machine m2(1);
		m1.matchedMachine = &m2;
		m2.matchedMachine = &m1;
		while (true)
		{
			m1.doInsn_2(program);
			m2.doInsn_2(program);
			if (m1.machineState == machine::state::awaitInput && m2.machineState == machine::state::awaitInput)
			{
				break;
			}
			if (m1.machineState == machine::state::stopped)
			{
				if (m2.machineState == machine::state::stopped)
				{
					break;
				}
				else if (m2.machineState == machine::state::awaitInput)
				{
					break;
				}

			}
			if (m2.machineState == machine::state::stopped)
			{
				if (m1.machineState == machine::state::stopped)
				{
					break;
				}
				else if (m1.machineState == machine::state::awaitInput)
				{
					break;
				}

			}
		}
		cout << m2.sent << "\n";
	}
	catch (FileReadException &e)
	{
		cerr << "Could not read file.\n";
	}
	return 0;
}

