/*
 * file_lock.h
 *
 *  Created on: 07/07/2010
 *      Author: nicolas
 */

#ifndef FILE_LOCK_H_
#define FILE_LOCK_H_

#include <fcntl.h>

/**
 * A class responsible for locking a file so only one process can access to it
 * at the same time. FileLock is advisory, so it's the programmer responsability
 * to use then when a file has one. FileLock locks the entire file for reading
 * and writing.
 */
class FileLock
{
public:
	/**
	 * Locks a file.
	 *
	 * @param fd 	File descriptor of the file to be locked.
	 */
	explicit FileLock(int fd);

	/**
	 * Unlocks the file.
	 */
	virtual ~FileLock();

private:
	int _fd;
	struct flock _lockInfo;
};

#endif /* FILE_LOCK_H_ */
