/*
 * config_file.h
 *
 *  Created on: Jul 12, 2010
 *      Author: demian
 */

#ifndef CONFIG_FILE_H_
#define CONFIG_FILE_H_

#include "config.h"

#include <map>

using std::map;

/**
 * A Unix-style configuration file.
 * Each non blank line of the file can be either a comment (if it starts with
 * '#') or a key-value pair o the form:
 * key = value
 */
class ConfigFile: public Config
{
public:
	ConfigFile(const string& fileName);
	virtual ~ConfigFile();

protected:
	virtual string getString(const string& key) const;

private:
	map<string, string> _data;
};

#endif /* CONFIG_FILE_H_ */
