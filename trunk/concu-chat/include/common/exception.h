/*
 * exception.h
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <exception>
#include <string>

using std::string;

/**
 * An exception with a stack trace!
 *
 * NOTE: Needs to be linked with the option -rdynamic under gcc in order to
 * have a legible stack trace.
 */
class Exception: public std::exception
{
public:
	explicit Exception(const string& msj = "");
	explicit Exception(const std::exception& e);
	virtual ~Exception() throw () { }

	virtual const char* what() const throw();

private:
	string _what;
};

#endif /* EXCEPTION_H_ */
