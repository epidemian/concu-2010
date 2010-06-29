/*
 * utils.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#include "utils.h"

#include <sstream>
#include <algorithm>

int strToInt(const char* str) throw (std::invalid_argument)
{
	int n;
	std::istringstream iss(str);
	iss >> n;
	if (!iss)
		throw std::invalid_argument("strToInt(): Bad format");
	return n;
}

string trimLeft(const string& str)
{
	size_t startPos = 0;
	while (startPos < str.size() && isblank(str[startPos]))
		startPos++;
	return startPos == 0 ? str : str.substr(startPos);
}

string trimRight(const string& str)
{
	size_t nChars = str.size();
	while (nChars > 0 && isblank(str[nChars - 1]))
		nChars--;
	return nChars == str.size() ? str : str.substr(0, nChars);
}

string trim(const string& str)
{
	return trimLeft(trimRight(str));
}

string toLowerCase(const string& str)
{
	string lower = str;
	for (size_t i = 0; i < str.size(); i++)
		lower[i] = tolower(str[i]);
	return lower;
}
