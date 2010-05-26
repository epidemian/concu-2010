/*
 * main.cpp
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#include "client.h"

int main(int argc, char* argv[])
{
	Client client(argc, argv);
	return client.run();

}


