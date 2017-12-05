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

#endif //LIB_H_