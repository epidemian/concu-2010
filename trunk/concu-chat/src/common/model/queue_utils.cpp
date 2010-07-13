/*
 * utils.cpp
 *
 *  Created on: 29/06/2010
 *      Author: nicolas
 */

#include "model/queue_utils.h"
#include "global_config.h"

#include <sstream>
#include <string>

using std::ostringstream;
using std::string;

const string QUEUE_DIRECTORY_LABEL = "queue_directory";
const string CLIENT_PREFIX_LABEL = "client_prefix";
const string QUEUE_EXTENSION_LABEL = "queue_extension";
const string SERVER_QUEUE_PATH_LABEL = "server_queue_path";

string getClientQueueFileName(pid_t pid)
{
	ostringstream oss;
	oss <<	GlobalConfig::get<string>(QUEUE_DIRECTORY_LABEL)
	    << GlobalConfig::get<string>(CLIENT_PREFIX_LABEL)
	    << pid
	    << GlobalConfig::get<string>(QUEUE_EXTENSION_LABEL);

	return oss.str();
}

string getServerQueueFileName()
{
	return GlobalConfig::get<string>(SERVER_QUEUE_PATH_LABEL);
}
