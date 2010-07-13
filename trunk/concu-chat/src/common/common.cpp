/*
 * arg_parse.cpp
 *
 *  Created on: Jul 12, 2010
 *      Author: demian
 */

#include "common.h"
#include "logger.h"
#include "config_file.h"
#include "global_config.h"

#include <getopt.h>
#include <cstdlib>
#include <iostream>
#include <cstring>

using std::cout;
using std::cerr;

namespace
{

static const string CONFIG_FILE_NAME = "config.txt";

void showHelp(const string& commandName)
{
	cout << "Usage: " << commandName << " [option]\n";
	cout << "\n";
	cout << "Options:\n";
	cout << "    -h, --help       Show this help message and exit\n";
	cout << "    -l, --enable-log Enables logging\n";
}

} // end namespace

void parseArguments(int argc, char* argv[])
{
	string commandName = basename(argv[0]);

	int c;
	opterr = 0;
	struct option longOptions[] =
	{
	{ "help", no_argument, 0, 'h' },
	{ "enable-log", no_argument, 0, 'l' },
	{ 0, 0, 0, 0 } };

	while ((c = getopt_long(argc, argv, ":hl", longOptions, NULL)) != -1)
	{
		switch (c)
		{
		case 'h':
			showHelp(commandName);
			exit(EXIT_SUCCESS);
		case 'l':
			Logger::instance().setLogging(true);
			break;
		case '?':
			cerr << "Invalid option: ";
			if (optopt != 0)
				cerr << char(optopt) << "\n";
			else
				cerr << argv[optind - 1] << "\n";
			showHelp(commandName);
			exit(EXIT_FAILURE);
		default:
			cerr << "Error parsing arguments\n";
			exit(EXIT_FAILURE);
		}
	}
}

char getQueueId(){
	return 'Q';
}

void loadConfigFile()
{
	static ConfigFile configFile(CONFIG_FILE_NAME);
	GlobalConfig::setConfig(&configFile);
}
