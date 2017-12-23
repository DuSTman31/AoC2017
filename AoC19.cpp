#include "Lib.h"
#include <iostream>
using std::cerr;
using std::cout;

enum class direction{north, east, south, west};

struct packet
{
	direction d = direction::south;
	int x;
	int y;
};

int findStartX(vector<string> &a)
{
	for (int i = 0; i!=a[0].length() ; i++)
	{
		if (a[0][i] == '|')
		{
			return i;
		}
	}
	return 0;
}

bool hasPath(packet &p, vector<string> &board, direction d)
{
	if (d == direction::north)
	{
		if (p.y == 0)
		{
			return false;
		}
		if (board[p.y - 1][p.x] == ' ')
		{
			return false;
		}
		return true;
	}
	if (d == direction::south)
	{
		if (p.y == (board.size() - 1))
		{
			return false;
		}
		if (board[p.y + 1][p.x] == ' ')
		{
			return false;
		}
		return true;
	}
	if (d == direction::east)
	{
		if (p.x == (board[p.y].size() - 1))
		{
			return false;
		}
		if (board[p.y][p.x + 1] == ' ')
		{
			return false;
		}
		return true;
	}
	if (d == direction::west)
	{
		if (p.x == 0)
		{
			return false;
		}
		if (board[p.y][p.x - 1] == ' ')
		{
			return false;
		}
		return true;
	}
	return false;
}

bool processChar(packet &p, vector<string> &board, vector<char> &log)
{
	if (board[p.y][p.x] == '+')
	{
		if (p.d == direction::north)
		{
			if (hasPath(p, board, direction::east))
			{
				p.d = direction::east;
			}
			else if (hasPath(p, board, direction::west))
			{
				p.d = direction::west;
			}
			
		}
		else if (p.d == direction::east)
		{
			if (hasPath(p, board, direction::north))
			{
				p.d = direction::north;
			}
			else if (hasPath(p, board, direction::south))
			{
				p.d = direction::south;
			}
		}
		else if (p.d == direction::south)
		{
			if (hasPath(p, board, direction::east))
			{
				p.d = direction::east;
			}
			else if (hasPath(p, board, direction::west))
			{
				p.d = direction::west;
			}
		}
		else if (p.d == direction::west)
		{
			if (hasPath(p, board, direction::north))
			{
				p.d = direction::north;
			}
			else if (hasPath(p, board, direction::south))
			{
				p.d = direction::south;
			}
		}

	}
	else if (board[p.y][p.x] >= 'A' && board[p.y][p.x] <= 'Z')
	{
		log.push_back(board[p.y][p.x]);
	}
	else if (board[p.y][p.x] == ' ')
	{
		return false;
	}

	if (p.d == direction::south)
	{
		p.y++;
	}
	else if (p.d == direction::east)
	{
		p.x++;
	}
	else if (p.d == direction::north)
	{
		p.y--;
	}
	else if (p.d == direction::west)
	{
		p.x--;
	}
	return true;
}

int main(int argc, char* argv[])
{
	try
	{
		vector<string> input = readFileAsLines("in_19.txt"); 

		int x = findStartX(input);
		cout << "Start x " << x << "\n";

		packet p;
		p.x = x;
		p.y = 0;

		vector<char> log;
		unsigned int dist = 0;
		while (processChar(p, input, log))
		{
			dist++;
		}
		for (auto c : log)
		{
			cout << c;
		}
		cout << "\n";
		cout << "distance " << dist << "\n";
 	}
	catch (FileReadException &e)
	{
		cerr << "Could not read file.\n";
	}
	return 0;
}

