
#include "Lib.h"
#include <iostream>
using std::cout;
using std::cerr;
#include <algorithm>
using std::for_each;
#include <cmath>

#include <map>
using std::map;

#include <pthread.h>

class FormatException{};

enum class direction{north, northeast, southeast, south, southwest, northwest};

direction parseDir(const string &in, unsigned int offset, unsigned int &consume)
{
	unsigned int i=0;
	bool j=false;
	if(j = matchString("nw", in, offset+i)){
		i += 2;
		consume = i;
		
		return direction::northwest;
	}
	if(j = matchString("ne", in, offset+i)){
		i += 2;
		consume = i;
		
		return direction::northeast;
	}
	if(j = matchString("n", in, offset+i)){
		i += 1;
		consume = i;
		return direction::north;
	}
	if(j = matchString("sw", in, offset+i)){
		i += 2;
		consume = i;
		
		return direction::southwest;
	}
	if(j = matchString("se", in, offset+i)){
		i += 2;
		consume = i;
		return direction::southeast;
	}
	if(j = matchString("s", in, offset+i)){
		i += 1;
		consume = i;
		return direction::south;
	}

	throw FormatException();
}

vector<direction> parseDirections(const string &in)
{
	vector<direction> res;
	unsigned int i=0;

	while(i < in.length())
	{
		unsigned int con = 0;
		res.push_back(parseDir(in, i, con));
		i+=con;

		if(i < in.length())
		{
			if(in[i] == ',')
			{
				i++;
			}
			else
			{
				throw FormatException();
			}
		}
	}
	return res;
}

class position{
public:
	void moveDirection(direction d)
	{
		switch(d){
		case direction::north:
			y -= 2;
			break;
		case direction::south:
			y += 2;
			break;
		case direction::northeast:
			y -= 1;
			x += 1;
			break;
		case direction::northwest:
			y -= 1;
			x -= 1;
			break;
		case direction::southeast:
			y += 1;
			x += 1;
			break;
		case direction::southwest:
			y += 1;
			x -= 1;
			break;
			
		};

	};

	void op()
	{
		cout << "Position x=" << x << " y=" << y << "\n"; 
	};

	bool operator==(const position &a)
	{
		if(a.x == x && a.y == y)
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	friend bool operator<(const position &a, const position &b)
	{
		if(a.x == b.x)
		{
			return a.y < b.y;
		}
		else {
			return a.x < b.x;
		}
	};
	
	int x =0;
	int y = 0;
};

class route
{
public:
	route expand(direction d)
	{
		route b;
		b.dist = dist+1;
		b.pos = pos;
		b.pos.moveDirection(d);
		return b;
	};
	void op()
	{
		cout << "Distance " << dist << " ";
		pos.op();
	};
	
	position pos;
	int dist = 0;
};

int distanceHeuristic(route &a, position &target)
{
	int dx = target.x - a.pos.x;
	int dy = target.y - a.pos.y;

	dx = abs(dx);
	dy = abs(dy);

	return sqrt(dx * dx + dy * dy) + a.dist;
};

void addIfNotPresent(vector<route> &vr, route nr, map<position, unsigned int> &posMap)
{
	if(posMap.count(nr.pos) == 0)
	{
		vr.push_back(nr);
		posMap[nr.pos] = vr.size()-1;
	}
	else if(vr[posMap[nr.pos]].dist > nr.dist)
	{
		vr[posMap[nr.pos]] = nr;
	}
}

route AStar(vector<route> &vr, position &target)
{
	map<position, unsigned int> posMap;
	while(true)
	{
		bool found = false;
		unsigned int candidateRoute =0;
		int candidateDist = 0;
		for(unsigned int i=0 ; i!=vr.size() ; i++)
		{
			int eDist = distanceHeuristic(vr[i], target);

			if(!found)
			{
				found = true;
				candidateRoute = i;
				candidateDist = eDist;
			}
			else if(eDist < candidateDist)
			{
				candidateRoute = i;
				candidateDist = eDist;
			}
		}
		
		route chosen = vr[candidateRoute];
		vr.erase(vr.begin() + candidateRoute);
		posMap.erase(chosen.pos);

		if(chosen.pos == target)
		{
			return chosen;
		}

		addIfNotPresent(vr, chosen.expand(direction::north), posMap);
		addIfNotPresent(vr, chosen.expand(direction::northwest), posMap);
		addIfNotPresent(vr, chosen.expand(direction::northeast), posMap);
		addIfNotPresent(vr, chosen.expand(direction::south), posMap);
		addIfNotPresent(vr, chosen.expand(direction::southeast), posMap);
		addIfNotPresent(vr, chosen.expand(direction::southwest), posMap);
		
	}
	
}

struct threadInfo
{
	int threadNo;
	vector<position> *in;
};

unsigned int tResults[4] = {0, 0, 0, 0};

void *threadStart(void *dat)
{
	threadInfo *myInfo = (threadInfo*)dat;

	unsigned int farthest = 0;

	for(unsigned int i=myInfo->threadNo ; i< myInfo->in->size() ; i+=4)
	{
		vector<position> *ps = myInfo->in;
		position p2 = (*ps)[i];
	    
		route b;
		vector<route> vr2;
		vr2.push_back(b);
		route path2 = AStar(vr2, p2);
		cout << "Path done " << i++ << " of " << myInfo->in->size() << "\n";
		if(path2.dist > farthest)
		{
			farthest = path2.dist;
		}
	}
	tResults[myInfo->threadNo] = farthest;
}

int main(int argc, char* argv[])
{
	try{
		string input = readWholeFile("in_11.txt");
		string trimmedInput = trimTrailingNewlines(input);
		
		vector<direction> dirs = parseDirections(trimmedInput);

		position p;
		for(auto d : dirs)
		{
			p.moveDirection(d);
		}
		p.op();

		route a;
		
		vector<route> vr;
		vr.push_back(a);

		route path = AStar(vr, p);
		cout << "Part 1 path dist " << path.dist << "\n";		       

		position p2;
		vector<position> positions;
		for(auto d: dirs)
		{
			p2.moveDirection(d);
			positions.push_back(p2);
		}

		unsigned int farthest = 0;
		pthread_t threads[4];
		for(unsigned int i=0 ; i!=4 ; i++)
		{
			threadInfo *ti = new threadInfo;
			ti->threadNo = i;
			ti->in = &positions;
			pthread_create(&threads[i], NULL, threadStart, ti );
		}

		for(unsigned int i=0 ; i!=4 ; i++)
		{
			pthread_join(threads[i], NULL);
			if(tResults[i] > farthest)
			{
				farthest = tResults[i];
			}
		}
		
		cout << "Part 2 max distance: " << farthest << "\n";
	}
	catch (FileReadException &e)
	{
		cerr << "Could not read file.\n";
	}
	catch(FormatException &e)
	{
		cerr << "Invalid format detected.\n";
	}
	return 0;
}

