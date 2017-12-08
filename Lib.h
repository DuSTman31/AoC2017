#ifndef LIB_H_
#define LIB_H_

#include <string>
using std::string;
#include <cstdio>
#include <vector>
using std::vector;

class FileReadException{};

string readWholeFile(const char *fileName){
	unsigned int fileSize = 0;
	FILE *fh = fopen(fileName, "rb");
	if (fh)
	{
		fseek(fh, 0, SEEK_END);
		fileSize = ftell(fh);
		fseek(fh, 0, SEEK_SET);
		char *buf = new char[fileSize];
		fread(buf, 1, fileSize, fh);
		fclose(fh);
		string bux;

		for (unsigned int i = 0 ; i != fileSize; i++)
		{
			bux.append(1, buf[i]); 
		}

		delete[] buf;
		return bux;
	}
	else
	{
		throw FileReadException();
	}
}

vector<string> splitIntoLines(string &in)
{
	string buf;
	vector<string> lines;

	for (unsigned int i = 0; i != in.length(); i++)
	{
		if (in[i] == '\n')
		{
			lines.push_back(buf);
			buf = string();
		}
		else if (in[i] == '\r')
		{
		}
		else
		{
			buf.append(1, in[i]);
		}
	}
	if (buf.length() > 0) {
		lines.push_back(buf);
	}

	return lines;
}

vector<string> readFileAsLines(const char* fileName)
{
	string buf = readWholeFile(fileName);
	return splitIntoLines(buf);
}

unsigned int consumeWhiteSpace(const string &a, unsigned int offset)
{
	unsigned int i = 0;
	while (offset + i != a.length())
	{
		char c = a[i + offset];
		if (c != ' ' && c != '\t')
		{
			return i;
		}
		i++;
	}
	return 0;
}

bool matchString(const string &needle, const string &haystack, unsigned int haystackOffset)
{
	unsigned int i = 0;
	for (; i!=needle.length() ; i++)
	{
		if (haystack[haystackOffset + i] != needle[i])
		{
			return false;
		}
	}
	return true;
}

int decodeInt(const string &buf, unsigned int offset, unsigned int *consume)
{
	unsigned int i = 0;
	int temp = 0;
	string obuf; 
	while ((buf[offset + i] >= '0' && buf[offset + i] <= '9') || buf[offset+i] == '-')
	{
		obuf.append(1, buf[offset + i]);
		i++;
	}
	*consume = i;
	return stoi(obuf, 0, 0);
}

#endif //LIB_H_