#include "Lib.h"

#include <iostream>
using std::cout;

int findGreatestBank(vector<int> &a)
{
	bool found = false;
	int ind = 0;
	for (int i = 0; i!=a.size() ; i++)
	{
		if (!found)
		{
			found = true;
			ind = i;
		}
		else if (a[i] > a[ind])
		{
			ind = i;
		}
	}
	return ind;

}

vector<int> redistribute(vector<int> &a, unsigned int col)
{
	vector<int> n = a;
	int colToRedist = a[col];
	n[col] = 0;
	col++;

	while (colToRedist > 0)
	{
		if (col == n.size())
		{
			col %= n.size();
		}

		n[col]++;
		colToRedist--;
		col++;
	}
	return n;
}

bool seenBefore(vector<vector<int>> &dat, vector<int> &needle)
{
	for (unsigned int i = 0; i != dat.size(); i++)
	{
		if (dat[i] == needle)
		{
			return true;
		}
	}
	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	vector<int> data = { 2, 8, 8, 5, 4, 2, 3, 1, 5, 5, 1, 2, 15, 13, 5, 14 };
	vector<int> db = data;

	vector<vector<int>> prevVersions;
	bool p2 = false;
	int noCycles = 0;
	while (true)
	{
		int gb = findGreatestBank(db);
		db = redistribute(db, gb);
		if (seenBefore(prevVersions, db))
		{
			noCycles++;
			if (!p2)
			{
				cout << "part 1 " <<noCycles << "\n";
				p2 = true;
				noCycles = 0;
				prevVersions.clear();
				prevVersions.push_back(db);
			}
			else
			{
				cout << "part 2 " << noCycles << "\n";
				break;
			}

		}
		else
		{
			prevVersions.push_back(db);
			noCycles++;
		}
	}

	return 0;
}

