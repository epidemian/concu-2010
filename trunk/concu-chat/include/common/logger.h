/*
 * logger.h
 *
 *  Created on: Jul 12, 2010
 *      Author: demian
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "utils.h"


class Logger
{
public:

	void log(const string& message);

	void setLogging(bool value);

	/** Singleton instance */
	static Logger& instance();

private:
	bool _logging;

	/**
	 * Log-file file descriptor.
	 * Note: raw file descriptors are used instead of FILE* or, preferably,
	 * std::ofstream because the FileLock needs a file descriptor in order to
	 * work.
	 */
	int _fd;

	Logger();

	~Logger();

	DECLARE_NON_COPIABLE(Logger)
};

#endif /* LOGGER_H_ */
