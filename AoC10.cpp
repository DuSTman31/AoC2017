#include "Lib.h"
#include <iostream>
using std::cout;
#include <algorithm>
using std::for_each;
 
class knot{
public:
  knot()
  {
    for(int i=0 ; i!=256 ; i++)
      {
	list.push_back(i);
      }
  }
  
  void doPass(int length)
  {
    unsigned int startPosition = currentPosition;
    unsigned int endPosition = startPosition + length - 1;

    while(startPosition < endPosition)
      {
	int temp = list[startPosition%list.size()];
	list[startPosition%list.size()] = list[endPosition%list.size()];
	list[endPosition%list.size()] = temp;
	endPosition--;
	startPosition++;
      }

    currentPosition += length;
    currentPosition += skip++;
    currentPosition %= list.size();
  }

  void op()
  {
    for(auto i : list)
      {
	cout << i << " ";
      }
    cout << "\n";
  }

  string denseHash()
  {
    string dh;
    int block = 0;
    int inBlock = 0;
    for(unsigned int i=0 ; i!=list.size() ;i++)
      {
	block ^= list[i];
	inBlock++;
	if(inBlock==16)
	  {
	    char pallette[] = "0123456789abcdef";
	    char high = (block&0xF0)>>4;
	    char low = block&0x0F;
	    
	    dh.append(1, pallette[high]);
	    dh.append(1, pallette[low]);
	    block = 0;
	    inBlock = 0;
	  }
      }
    return dh;
  }
private:
  
  unsigned int currentPosition = 0;
  unsigned int skip = 0;
  vector<int> list;
};

void round(vector<int> &lengths, knot &k)
{
  for(auto i : lengths)
    {
      k.doPass(i);
    }
}

int main(int argc, char* argv[])
{
  vector<int> data = {14,58,0,116,179,16,1,104,2,254,167,86,255,55,122,244};
  string dataString = "14,58,0,116,179,16,1,104,2,254,167,86,255,55,122,244";
  //vector<int> data = {3, 4, 1, 5};

  knot k;
  for(auto i : data)
    {
      k.doPass(i);
    }
  cout << "Part 1 ";
  k.op();

  vector<int>  processedData;
  for(char i : dataString)
    {
      if(i >= '0' && i <= '9')
	{
	  processedData.push_back(i);
	}
      else if(i == ',')
	{
	  processedData.push_back(i);
	}
    }
  processedData.push_back(17);
  processedData.push_back(31);
  processedData.push_back(73);
  processedData.push_back(47);
  processedData.push_back(23);

  knot k2;
  for(unsigned int i=0 ; i!=64 ; i++)
    {
      round(processedData, k2);
    }

  cout << "Part 2 Dense hash " << k2.denseHash() << "\n";
  return 0;
}

