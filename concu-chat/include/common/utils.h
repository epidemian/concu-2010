/*
 * utils.h
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdexcept>
#include <string>
#include <sstream>

using std::string;

/**
 * Nice macro to get the size of a static array.
 */
#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

/**
 * An other nice macro to declare a class non copiable. i.e: declares a private
 * copy constructor and assignment operator. Users of this macro should not
 * define the copy constructor not the assignment operator.
 */
#define DECLARE_NON_COPIABLE(className) private: \
	className(className&); \
	void operator = (className&);

/**
 * Transforms a string to an integer. Equivalent to atoi function, except this
 * one throws a exception in case of a bad format.
 */
int strToInt(const char* str) throw (std::invalid_argument);

/**
 * Transforms any object of type T to a string. The operator << (ostring&, T)
 * must be defined.
 */
template<typename T> string toStr(T obj)
{
	std::ostringstream oss;
	oss << obj;
	return oss.str();
}

/**
 * Removes the leading whitespace of a string.
 * e.g: trimLeft("  hello world ") returns "hello world "
 */
string trimLeft(const string& str);

/**
 * Removes the trailing whitespace of a string.
 * e.g: trimRight(" hello world  ") returns " hello world"
 */
string trimRight(const string& str);

/**
 * Removes the leading and trailing whitespace of a string.
 * e.g: trim("  hello world  ") return "hello world"
 */
string trim(const string& str);

#endif /* UTILS_H_ */
