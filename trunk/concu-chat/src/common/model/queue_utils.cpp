/*
 * utils.cpp
 *
 *  Created on: 29/06/2010
 *      Author: nicolas
 */

#include "model/queue_utils.h"

#include <sstream>

using std::ostringstream;

string getClientQueueFileName(pid_t pid)
{
	ostringstream oss;
	oss << "queues/client" << pid << ".queue";
	return oss.str();
}

string getServerQueueFileName()
{
	return "queues/server.queue";
}
