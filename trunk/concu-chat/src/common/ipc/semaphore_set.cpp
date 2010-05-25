/*
 * semaphore.cpp
 *
 *  Created on: Apr 16, 2010
 *      Author: demian
 */

#include "ipc/semaphore_set.h"
#include "ipc/ipc_error.h"

#include <errno.h>
#include <sys/sem.h>

#include <ctime>
#include <cstdio>


static const int SEM_TIMEOUT = 5; // In seconds.

// Helper functions.

/**
 * Initializes a System V semaphore set, taking care of the race conditions
 * there might occur.
 *
 * If the semaphore set was already created by an other program, the initial
 * values vector is ignored.
 *
 * More-than-inspired by W. Richard Stevens' UNIX Network Programming 2nd
 * edition, volume 2, lockvsem.c, page 295.
*/
int initSemSet(const string& pathName, char id,
		const SemaphoreSet::InitValues& initVals)
{

	// Some helper functions declarations.
	void initNewSemSet(int semId, const SemaphoreSet::InitValues& initVals);
	int  waitForSemInitialization(key_t key, size_t nSems);

	// Creates key.
	key_t key = ftok(pathName.c_str(), id);
	if (key == (key_t)-1)
		throw IpcError("initSemSet(): Could not create key", errno);

	const size_t nSems = initVals.size();

	// Uses IPC_CREAT | IPC_EXCL in order to fail if the semaphore set already
	// exists.
	int semId = semget(key, nSems, IPC_CREAT | IPC_EXCL | 0666);

	if (semId >= 0) // Great, we got it first.
	{
		initNewSemSet(semId, initVals);
	}
	else if (errno == EEXIST) // Someone got it first.
	{
		semId = waitForSemInitialization(key, nSems);
	}
	else
		throw IpcError("initSemSet(): semget() failed.", errno);

	return semId;
}


/** semun defined as man page. */
union semun
{
   int              val;    /* Vafirstlue for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO
							   (Linux-specific) */
};


void initNewSemSet(int semId, const SemaphoreSet::InitValues& initVals)
{
	const size_t nSems = initVals.size();

	// Initializes all values to zero first (in Linux this is not needed, but,
	// in general, it is).
	union semun arg;
	SemaphoreSet::InitValues zeros(nSems, 0);
	arg.array = &zeros[0];

	if (semctl(semId, 0, SETALL, arg) == -1)
	{
		int e = errno;
		semctl(semId, 0, IPC_RMID); // Clean up.
		throw IpcError("initNewSemSet(): semctl() failed", e);
	}

	// Now initializes values to the initial values using semop, wich "frees"
	// other SemaphoreSets. This sets the sem_otime to something != 0, as needed
	// in waitForSemInitialization().
	for (size_t i = 0; i < nSems; ++i)
	{
		struct sembuf op;
		// Moves the semaphore up initVals[i] times (if initVals[i] == 0,
		// nothing is done, as the state of the i'th semaphore will be 0)
		op.sem_op = initVals[i];
		op.sem_num = i;
		op.sem_flg = 0;

		if (semop(semId, &op, 1) == -1)
		{
			int e = errno;
			semctl(semId, 0, IPC_RMID); // Clean up.
			throw IpcError("initNewSemSet(): semop() failed", e);
		}
	}
}


int waitForSemInitialization(key_t key, size_t nSems)
{
	// Gets the semaphore set id without trying to create it.
	int semId = semget(key, nSems, 0666);
	if (semId == -1)
		throw IpcError("waitForSemInitialization(): semget() failed.", errno);

	// Waits until other process initializes semaphore set.
	bool ready = false;
	clock_t initClock = clock();
	while (!ready)
	{
		struct semid_ds buf;
		union semun arg;
		arg.buf = &buf;
		if (semctl(semId, nSems - 1, IPC_STAT, arg) == -1)
			throw IpcError("waitForSemInitialization(): semctl() failed",
					errno);

		bool timeout = (clock() - initClock) > SEM_TIMEOUT * CLOCKS_PER_SEC;
		if (timeout)
			throw IpcError("waitForSemInitialization(): Timed out");

		ready = arg.buf->sem_otime != 0;
	}
	return semId;
}


int semOperation(int semId, size_t semIndex, int op, int flags = 0)
{
	struct sembuf operation;
	operation.sem_num = semIndex;
	operation.sem_op  = op;
	operation.sem_flg = flags;

	return semop(semId, &operation, 1);
}

// End helper functions.

SemaphoreSet::SemaphoreSet(const string& pathName, char id,
		size_t nSems, int initVals[], bool ownResources) :
	Resource(ownResources),
	_semId(initSemSet(pathName, id, InitValues(initVals, initVals + nSems))),
	_nSems(nSems)
{ }


SemaphoreSet::SemaphoreSet(const string& pathName, char id,
		const InitValues& initVals, bool ownResources) :
	Resource(ownResources),
	_semId(initSemSet(pathName, id, initVals)),
	_nSems(initVals.size())
{ }


SemaphoreSet::~SemaphoreSet() throw ()
{
	doDispose();
}

void SemaphoreSet::wait(size_t semIndex)
{
	validateIndex(semIndex);
	if (semOperation(_semId, semIndex, -1) == -1)
		throw IpcError("SemaphoreSet::signal(): wait() failed", errno);
}


void SemaphoreSet::signal(size_t semIndex)
{
	validateIndex(semIndex);
	if (semOperation(_semId, semIndex, 1) == -1)
		throw IpcError("SemaphoreSet::signal(): semop() failed", errno);
}


bool SemaphoreSet::tryWait(size_t semIndex)
{
	validateIndex(semIndex);
	if (semOperation(_semId, semIndex, -1, IPC_NOWAIT) == -1)
	{
		if (errno == EAGAIN)
			return false;
		else
			throw IpcError("SemaphoreSet::tryWait(): semop() failed", errno);
	}
	else
		return true;
}


void SemaphoreSet::validateIndex(size_t semIndex)
{
	if (semIndex >= _nSems)
		throw IpcError("SemaphoreSet::validateIndex(): Index out of range");
}


SemaphoreProxy SemaphoreSet::getSemaphore(size_t semIndex)
{
	validateIndex(semIndex);
	return SemaphoreProxy(*this, semIndex);
}

void SemaphoreSet::print(ostream& stream) const
{
	stream << "semaphore set (id = " << _semId << ", nsems = " << _nSems
	       << ")";
}


void SemaphoreSet::doDispose() throw ()
{
	if (ownResources() && semctl(_semId, 0, IPC_RMID) == -1)
		perror("~SemaphoreSet(): Could not remove semaphore set");
}


