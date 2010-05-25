/*
 * person.h
 *
 *  Created on: Apr 9, 2010
 *      Author: demian
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <cstring>

class Person
{
public:
	explicit Person(const char* name = "", int age = 0)
	{
		setName(name);
		_age = age;
	}

	const char* name() { return _name; }
	int         age()  { return _age; }

	void becomeOlder() { _age++; }

private:

	static const size_t NAME_SIZE = 256;

	char _name[NAME_SIZE];
	int  _age;

	void setName(const char* name)
	{
		strncpy(_name, name, NAME_SIZE);
	}

};

#endif /* PERSON_H_ */
