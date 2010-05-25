/*
 * random.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#include "random.h"

#include <cstdlib>
#include <ctime>

// A little hackery...
class Randomizer
{
	friend int randomInt(int n);

	Randomizer()
	{
		srandom(time(NULL));
	}
};

int randomInt(int n)
{
	static Randomizer rand; // Created only once ;)
	return random() % n;
}
