/*
 * shared_memoty.cpp
 *
 *  Created on: Apr 9, 2010
 *      Author: demian
 */

#include "ipc/shared_memory.h"
#include "ipc/ipc_error.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include <cstdio>

RawSharedMemory::RawSharedMemory(size_t size, const string& pathName, char id,
		bool ownResources):
	Resource(ownResources)
{
	// Creates key.
	key_t key = ftok(pathName.c_str(), id);
	if (key == (key_t)-1)
		throw IpcError("SharedMemory(): Could not create key", errno);

	// Allocates a shared memory segment.
	_shmId = shmget(key, size, 0644 | IPC_CREAT);
	if (_shmId == -1)
		throw IpcError("SharedMemory(): "
				"Could not allocate shared memory", errno);

	// Attaches the shared memory to some address.
	_data = shmat(_shmId, NULL, 0);
	if (_data == (void*)-1)
		throw IpcError("SharedMemory(): "
				"Could not attach shared memory", errno);
}

RawSharedMemory::~RawSharedMemory() throw ()
{
	doDispose();
}

void RawSharedMemory::print(ostream& stream) const
{
	stream << "raw shared memory (id = " << _shmId << ")";
}

void RawSharedMemory::doDispose() throw ()
{
	int errorCode = shmdt(_data);
	if (errorCode == -1)
		// Throwing exceptions on destruction is not recommended.
		perror("~SharedMemory(): Could not detach shared memory");

	shmid_ds state;
	errorCode = ::shmctl(_shmId, IPC_STAT, &state);
	if (errorCode == -1)
		perror("~SharedMemory() Could not get shared memory state");

	if (state.shm_nattch == 0 && ownResources())
	{
		// Destroys shared memory.
		errorCode = shmctl(_shmId, IPC_RMID, NULL);
		if (errorCode == -1)
			perror("~SharedMemory(): Could not destroy shared memory");
	}
}

