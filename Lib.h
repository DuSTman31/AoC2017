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

		for (unsigned int i = 0; i != fileSize; i++)
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
	for (; i != needle.length(); i++)
	{
		if (haystack[haystackOffset + i] != needle[i])
		{
			return false;
		}
	}
	return true;
}

bool matchString(const char *needle, const string &haystack, unsigned int haystackOffset)
{
	unsigned int i = 0;
	for (; needle[i] != '\0'; i++)
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
	string obuf;
	while ((i < buf.length()) && ((buf[offset + i] >= '0' && buf[offset + i] <= '9') || buf[offset + i] == '-'))
	{
		obuf.append(1, buf[offset + i]);
		i++;
	}
	*consume = i;
	if (obuf.length() > 0)
	{
		return stoi(obuf, 0, 0);
	}
	else
	{
		return 0;
	}

}

vector<string> split(const string &in)
{
	unsigned int i = 0;
	vector<string> results;
	string buf;

	while (i<in.length())
	{
		i += consumeWhiteSpace(in, i);

		while (i<in.length() && in[i] != ' ' && in[i] != '\t')
		{
			buf.append(1, in[i]);
			i++;
		}

		if (buf.length() != 0)
		{
			results.push_back(buf);
			buf = string();
		}

	}
	return results;
}

vector<string> splitComma(const string &in)
{
	unsigned int i = 0;
	vector<string> results;
	string buf;

	while (i<in.length())
	{
		while (i<in.length() && in[i] != ',')
		{
			buf.append(1, in[i]);
			i++;
		}

		if (buf.length() != 0)
		{
			results.push_back(buf);
			buf = string();
		}
		i ++;
	}
	if (buf.length() != 0)
	{
		results.push_back(buf);
		buf = string();
	}
	return results;
}

string stripBrackets(string &a)
{
	string out;
	if (a[0] == '(' && a[a.length() - 1] == ')')
	{
		out = a.substr(1, a.length() - 2);
	}
	return out;
}

string trimTrailingNewlines(const string &a)
{
	int toSkip = 0;

	for (int i = a.length() - 1; i != -1; i--)
	{
		if (a[i] == '\n' || a[i] == '\r')
		{
			toSkip++;
		}
		else
		{
			break;
		}
	}

	return a.substr(0, a.length() - toSkip);
}

string trimTrailingCommas(const string &a)
{
	int toSkip = 0;

	for (int i = a.length() - 1; i != -1; i--)
	{
		if (a[i] == ',')
		{
			toSkip++;
		}
		else
		{
			break;
		}
	}

	return a.substr(0, a.length() - toSkip);
}

string trimTrailingColons(const string &a)
{
	int toSkip = 0;

	for (int i = a.length() - 1; i != -1; i--)
	{
		if (a[i] == ':')
		{
			toSkip++;
		}
		else
		{
			break;
		}
	}

	return a.substr(0, a.length() - toSkip);
}

#endif //LIB_H_
