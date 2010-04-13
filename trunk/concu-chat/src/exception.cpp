/*
 * exception.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#include "exception.h"

#include <sstream>
#include <cstdlib>

#include <execinfo.h>

using std::ostringstream;


static const size_t MAX_STACK_DEPTH = 100;

string generateStackTraceMessage(const string& msj)
{
	// Acquire the stack trace.
	void* stackAddrs[MAX_STACK_DEPTH];
	size_t stackDepth = backtrace(stackAddrs, MAX_STACK_DEPTH);
	char** stackStrings = backtrace_symbols(stackAddrs, stackDepth);

	// Probable out of memory condition. Doesn't matter =P
	if (stackStrings == 0)
		stackDepth = 0;

	ostringstream stackTraceBuf;
	for (size_t i = 0; i < stackDepth; ++i)
		stackTraceBuf << "\tAt " << stackStrings[i] << "\n";

	// Free up the allocated memory.
	free(stackStrings);

	return msj + "\n" + stackTraceBuf.str();
}


Exception::Exception(const string& msj): _what(generateStackTraceMessage(msj))
{ }


Exception::Exception(const std::exception& e):
		_what(generateStackTraceMessage(e.what()))
{ }

const char* Exception::what() const throw()
{
	return _what.c_str();
}
