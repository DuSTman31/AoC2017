#include "Lib.h"
#include <iostream>
using std::cerr;
using std::cout;
#include <algorithm>
using std::sort;
using std::unique;
#include <map>
using std::map;

class FormatException{};

struct coord
{
	void op()
	{
		cout << "x: " << x << " y: " << y << " z: " << z;
	}


	int x;
	int y;
	int z;
};

struct particle
{
	void op()
	{
		p.op();
		cout << "\t";
		v.op();
		cout << "\t";
		a.op();
		cout << "\n";
	}
	void simulate()
	{
		v.x += a.x;
		v.y += a.y;
		v.z += a.z;

		p.x += v.x;
		p.y += v.y;
		p.z += v.z;
	}
	int manhattanDistance()
	{
		return abs(p.x) + abs(p.y) + abs(p.z);
	}
	coord p;
	coord v;
	coord a;
};

inline bool operator< (const coord &a, const coord &b)
{
	if (a.x < b.x)
	{
		return true;
	}
	else if (a.x > b.x)
	{
		return false;
	}
	else
	{
		if (a.y < b.y)
		{
			return true;
		}
		else if (a.y > b.y)
		{
			return false;
		}
		else
		{
			if (a.z < b.z)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

coord parseCoord(const string &in, unsigned int offset, unsigned int &consume)
{
	coord n;

	unsigned int i = 0;
	if (in[offset] != '<')
	{
		throw FormatException();
	}

	i++;
	int temp = 0;
	unsigned int con = 0;
	temp = decodeInt(in, offset + i, &con);
	n.x = temp;
	i += con;

	while (in[offset + i] == ',')
	{
		i++;
	}

	con = 0;
	temp = decodeInt(in, offset + i, &con);
	n.y = temp;
	i += con;

	while (in[offset + i] == ',')
	{
		i++;
	}

	con = 0;
	temp = decodeInt(in, offset + i, &con);
	n.z = temp;
	i += con;

	consume = i;
	return n;
}

particle parseLine(const string &in)
{
	particle p;

	vector<string> sIn = split(in);

	unsigned int consume = 0;
	p.p = parseCoord(sIn[0], 2, consume);

	p.v = parseCoord(sIn[1], 2, consume);

	p.a = parseCoord(sIn[2], 2, consume);

	return p;
}

map<coord,int> countOccurrences(vector<particle> &parts)
{
	map<coord, int> counts;

	for (auto &p : parts)
	{
		if (counts.count(p.p) == 0)
		{
			counts[p.p] = 1;
		}
		else
		{
			counts[p.p]++;
		}
	}

	return counts;
}

void removeDups(vector<particle> &parts, map<coord, int> &ir)
{
	vector<particle> t;

	for (auto &p : parts)
	{
		if (ir[p.p] == 1)
		{
			t.push_back(p);
		}

	}
	parts = t;
}


int main(int argc, char* argv[])
{
	try
	{
		vector<string> input = readFileAsLines("in_20.txt");

		vector<particle> ps;
		for (auto &l : input)
		{
			ps.push_back(parseLine(l));
		}


		auto pbIter = ps.begin();
		auto peIter = ps.end();
		for (unsigned int i = 0; i!=1000; i++)
		{
			for (auto &p : ps)
			{
				p.simulate();
			}
			map<coord, int> counts;
			counts = countOccurrences(ps);
			removeDups(ps, counts);
		}

		bool particleFound = false;
		unsigned int fpInd = 0;
		for (unsigned int i = 0; i!=ps.size() ; i++)
		{
			if (!particleFound)
			{
				particleFound = true;
				fpInd = i;
			}
			else if (ps[i].manhattanDistance() < ps[fpInd].manhattanDistance())
			{
				fpInd = i;
			}
		}

		cout << "Particle no : " << fpInd << "\n";

		cout << "Particles remaining : " << ps.size() << "\n";
	}
	catch (FileReadException &e)
	{
		cout << "Could not read file. \n";
	}
	return 0;
}

