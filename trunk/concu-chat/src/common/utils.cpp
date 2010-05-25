/*
 * utils.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#include "utils.h"

#include <sstream>

int strToInt(const char* str) throw (std::invalid_argument)
{
	int n;
	std::istringstream iss(str);
	iss >> n;
	if (!iss)
		throw std::invalid_argument("strToInt(): Invalid argument");
	return n;
}
