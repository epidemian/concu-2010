/*
 * main.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */
/*
#include "tests.h"
#include "utils.h"
#include "random.h"

#include <iostream>

using std::cerr;

int main(int argc, char* argv[])
{
	TestFunction tests[] = {
	                         sharedMemoryTest,
	                         fifoTest,
	                         exceptionsTest,
	                       };
	size_t nTests = ARR_SIZE(tests);

	size_t index = argc == 2 ? strToInt(argv[1])
	                         : randomInt(nTests);

	if (index >= nTests)
	{
		cerr << "Invalid index\n";
		return 1;
	}
	else {
		try {
			return tests[index](argc, argv);
		}
		catch(...)
		{
			throw; // To force stack-unwinding.
		}
	}
	return 0;
}
*/
