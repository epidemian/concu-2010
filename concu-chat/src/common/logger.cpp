/*
 * logger.cpp
 *
 *  Created on: Jul 12, 2010
 *      Author: demian
 */

#include "logger.h"
#include "ipc/ipc_error.h"
#include "ipc/file_lock.h"

#include <ctime>
#include <cstdio>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/**
 * TODO: Get from config.
 */
static const string LOG_FILE_NAME = "log.txt";

namespace
{

static const size_t BUFFER_SIZE = 256;

string getTimeString()
{
	time_t rawTime = time(NULL);
	struct tm* timeInfo = localtime(&rawTime);
	char buffer[BUFFER_SIZE];

	strftime(buffer, BUFFER_SIZE, "%c", timeInfo);

	return string(buffer);
}

} // end namespace


Logger& Logger::instance()
{
	static Logger instance;
	return instance;
}

Logger::Logger() :
	_logging(true)
{
	_fd = open(LOG_FILE_NAME.c_str(), O_RDWR | O_APPEND | O_CREAT, 0644);

	if (_fd == -1)
		throw IpcError("Could not open log file " + LOG_FILE_NAME, errno);
}

Logger::~Logger()
{
	if (close(_fd) == -1)
		perror("~Logger(): Could not close log file");
}

void Logger::log(const string& message)
{
	if (_logging)
	{
		//FileLock lock(_fd);
		string logLine = getTimeString() + " - " + message + "\n";
		if (write(_fd, logLine.c_str(), logLine.size()) == -1)
			throw IpcError("Could not write log file ", errno);
	}
}

void Logger::setLogging(bool value)
{
	_logging = value;
}
