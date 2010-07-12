/*
 * config_file.cpp
 *
 *  Created on: Jul 12, 2010
 *      Author: demian
 */

#include "config_file.h"
#include "utils.h"

#include <fstream>

using std::ifstream;

namespace
{

void readConfigData(map<string, string>& data,
		ifstream& configFile)
{
	string line;

	while (getline(configFile, line))
	{
		// Skip comments
		if (trimLeft(line)[0] == '#')
			continue;

		size_t eqPos = line.find('=');
		if (eqPos == line.npos)
			continue;

		std::string key = trim(line.substr(0, eqPos));
		std::string value = trim(line.substr(eqPos + 1));

		data[key] = value;
	}
}

} // end namespace

ConfigFile::ConfigFile(const string& fileName)
{
	std::ifstream configFile(fileName.c_str());

	if (configFile.is_open())
		readConfigData(_data, configFile);
	else
		throw Exception("Could not open config file " + fileName);
}

ConfigFile::~ConfigFile()
{
}

string ConfigFile::getString(const string& key) const
{
	map<string, string>::const_iterator it = _data.find(key);
	if (it == _data.end())
		throw Exception("Invalid key: " + key);
	else
		return it->second;
}
