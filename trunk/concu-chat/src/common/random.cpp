/*
 * random.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#include "random.h"

#include <stdexcept>
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

int randomInt(int min, int max)
{
	if (max < min)
		throw std::invalid_argument("max must be greater than or equal to min");
	return min + randomInt(max - min + 1);
}
