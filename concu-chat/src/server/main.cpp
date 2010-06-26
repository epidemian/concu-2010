/*
 * main.cpp
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#include "server.h"

int main(int argc, char* argv[])
{
	Server server(argc, argv);
	return server.run();
}
