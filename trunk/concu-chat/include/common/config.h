/*
 * config.h
 *
 *  Created on: Jul 12, 2010
 *      Author: demian
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "exception.h"

#include <string>
#include <sstream>

using std::string;
using std::istringstream;

/**
 * An abstract configuration that maps string keys with values.
 */
class Config
{
public:
	virtual ~Config();

	/**
	 * Retrieves the value of type T associated to a given key.
	 */
	template<typename T>
	T get(const string& key)
	{
		string value = getString(key);
		istringstream iss(value);
		T ret;
		iss >> ret;

		if (iss.fail())
			throw Exception("Error reading value. Key = " + key + " value = "
					+ value);

		return ret;
	}

protected:
	Config();

	virtual string getString(const string& key) const = 0;
};

#endif /* CONFIG_H_ */
