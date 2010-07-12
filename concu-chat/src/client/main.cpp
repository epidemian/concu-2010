/*
 * main.cpp
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#include "client.h"
#include "arg_parse.h"

#include <iostream>

int main(int argc, char* argv[])
{
	parseArguments(argc, argv);

	try
	{
		Client client(argc, argv);
		return client.run();

	} catch (std::exception& e)
	{
		std::cerr << "Exception thrown " << e.what() << "\n";
	} catch (...)
	{
		std::cerr << "Unknown error\n";
	}
}

