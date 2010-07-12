/*
 * main.cpp
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#include "server.h"
#include "common.h"

#include <iostream>

int main(int argc, char* argv[])
{
	loadConfigFile();
	parseArguments(argc, argv);

	try
	{
		Server server;
		return server.run();

	} catch (std::exception& e)
	{
		std::cerr << "Exception thrown " << e.what() << "\n";
	} catch (...)
	{
		std::cerr << "Unknown error\n";
	}
}

