/*
 * exception.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#include "exception.h"

#include <sstream>
#include <cstdlib>
#include <iostream>

#include <execinfo.h>
#include <cxxabi.h>

using std::ostringstream;


static const size_t MAX_STACK_DEPTH = 100;

string demangleStackString(const string& str)
{
	// Find the parentheses and address offset surrounding the mangled name.
	size_t begin = str.find('(');
	size_t end   = str.find('+');

	string ret;
	if (begin != str.npos &&
	    end != str.npos &&
	    end > begin &&
	    begin != str.size() - 1)
	{
		// Found our mangled name.
		string mangledName = str.substr(begin + 1, end - begin - 1);
		string fileName = str.substr(0, begin);

		// Calls C++ ABI for demangling (quite hacky... but very nice!).
		int status;
		char* demangledName =
				abi::__cxa_demangle(mangledName.c_str(), NULL, NULL, &status);

		ret = demangledName ? fileName + ":" + demangledName
		                    : fileName + ":" + mangledName + "()";

		free(demangledName);
	}
	else {
		// Didn't find the mangled name. Returns the whole line.
		ret = str;
	}

	return ret;
}

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
	// Starts from 2 because 1st and 2nd lines are generateStackTraceMessage()
	// and Exception::Exception() respectively.
	for (size_t i = 2; i < stackDepth; ++i)
	{
		string demangledString = demangleStackString(stackStrings[i]);
		stackTraceBuf << "\tat " << demangledString << "\n";
	}

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
