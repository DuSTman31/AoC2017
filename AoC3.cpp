// AoC3.cpp : Defines the entry point for the console application.
//


#include <vector>
using std::vector;
#include <iostream>
using std::cout;


struct coords{
	int x;
	int y;
};

coords dirCoords[4] = { { 1, 0 }, { 0, -1 }, { -1, 0 }, { 0, 1 } };

int dist(unsigned int a)
{
	int i = 0;
	coords co = { 0, 0 };
	int stride = 1;
	int dir = 0;

	i++;
	while (true)
	{

		for (int k = 0; k != stride; k++, i++)
		{
			if (i == a)
			{
				return abs(co.x) + abs(co.y);
			}

			co.x += dirCoords[dir].x;
			co.y += dirCoords[dir].y;

		}
		dir++;
		dir %= 4;

		for (int k = 0; k != stride; k++, i++)
		{
			if (i == a)
			{
				return abs(co.x) + abs(co.y);
			}

			co.x += dirCoords[dir].x;
			co.y += dirCoords[dir].y;

		}
		dir++;
		dir %= 4;

		stride++;
	}
}

unsigned int coorToOff(int x)
{
	return x + 500;
}

int doSum(vector<vector<int>> &dat, coords &co)
{
	int total = 0;
	int startX = co.x - 1;
	int startY = co.y - 1;
	int endX = co.x + 1;
	int endY = co.y + 1;

	for (unsigned int i = 0 ; i!=3 ; i++)
	{
		for (unsigned int j = 0 ; j!=3 ; j++)
		{
			total += dat[coorToOff(startX + i)][coorToOff(startY + j)];

		}
	}
	return total;
}

int fill(unsigned int a, vector<vector<int>> &dat)
{
	int i = 0;
	coords co = { 0, 0 };
	int stride = 1;
	int dir = 0;

	i++;
	dat[coorToOff(co.x)][coorToOff(co.y)] = 1;

	while (true)
	{

		for (int k = 0; k != stride; k++, i++)
		{
			int sum = doSum(dat, co);
			dat[coorToOff(co.x)][coorToOff(co.y)] = sum;
			if (sum > a)
			{
				return sum;
			}

			co.x += dirCoords[dir].x;
			co.y += dirCoords[dir].y;

		}
		dir++;
		dir %= 4;

		for (int k = 0; k != stride; k++, i++)
		{
			int sum = doSum(dat, co);
			dat[coorToOff(co.x)][coorToOff(co.y)] = sum;
			if (sum > a)
			{
				return sum;
			}

			co.x += dirCoords[dir].x;
			co.y += dirCoords[dir].y;

		}
		dir++;
		dir %= 4;

		stride++;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Part 1 " << dist(325489) << "\n";

	vector<vector<int>> dat(1000, vector<int>(1000));

	for (unsigned int i = 0; i!=1000 ; i++)
	{
		for (unsigned int j = 0; j != 1000; j++)
		{
			dat[i][j] == 0;
		}
	}

	int u = fill(325489, dat);
	cout << "Part 2 " << u << "\n";

	return 0;
}

