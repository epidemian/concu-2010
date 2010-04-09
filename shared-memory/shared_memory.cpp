/*
 * shared_memoty.cpp
 *
 *  Created on: Apr 9, 2010
 *      Author: demian
 */

#include "shared_memory.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include <cstring>
#include <cstdio>

SharedMemoryError::SharedMemoryError(const string& msj, int errorCode) :
	runtime_error(msj + " - Error: " + (errorCode ? strerror(errorCode) : ""))
{ }

RawSharedMemory::RawSharedMemory(size_t size, const string& pathName, char id,
		bool freeOnExit)
{
	_freeOnExit = freeOnExit;

	// Creates key.
	key_t key = ftok(pathName.c_str(), id);
	if (key == (key_t)-1)
		throw SharedMemoryError("SharedMemory(): Could not create key", errno);

	// Allocates a shared memory segment.
	_shmId = shmget(key, size, 0644 | IPC_CREAT);
	if (_shmId == -1)
		throw SharedMemoryError("SharedMemory(): "
				"Could not allocate shared memory", errno);

	// Attaches the shared memory to some address.
	_data = shmat(_shmId, NULL, 0);
	if (_data == (void*)-1)
		throw SharedMemoryError("SharedMemory(): "
				"Could not attach shared memory", errno);
}

RawSharedMemory::~RawSharedMemory()
{
	int errorCode = shmdt(_data);
	if (errorCode == -1)
		// Throwing exceptions on destruction is not recommended.
		perror("~SharedMemory(): Could not detach shared memory");

	shmid_ds state;
	errorCode = ::shmctl(_shmId, IPC_STAT, &state);
	if (errorCode == -1)
		perror("~SharedMemory() Could not get shared memory state");

	if (state.shm_nattch == 0 && _freeOnExit)
	{
		// TODO: qué pasa si justo acá el scheduler cambia a otro proceso y
		// ese proceso hace un shmat de esta memoria?

		// Destroys shared memory.
		errorCode = shmctl(_shmId, IPC_RMID, NULL);
		if (errorCode == -1)
			perror("~SharedMemory(): Could not destroy shared memory");
	}
}
