/*
 * file_lock.cpp
 *
 *  Created on: 07/07/2010
 *      Author: nicolas
 */

#include "ipc/file_lock.h"
#include "ipc/ipc_error.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

FileLock::FileLock(int fd) : _fd(fd)
{
	_lockInfo.l_type = F_RDLCK | F_WRLCK;
	_lockInfo.l_whence = SEEK_SET;
	_lockInfo.l_start = 0;
	_lockInfo.l_len = 0;
	_lockInfo.l_pid = getpid();

	if (fcntl(_fd, F_SETLKW, &_lockInfo) == -1)
		throw IpcError("FileLock(): Could not acquire the lock", errno);
}

FileLock::~FileLock()
{
	_lockInfo.l_type = F_UNLCK;
	if (fcntl(_fd, F_SETLKW, &_lockInfo) == -1)
		perror("FileLock(): Could not release the lock");
}
