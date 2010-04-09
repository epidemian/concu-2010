/*
 * shared_memory.h
 *
 *  Created on: Apr 8, 2010
 *      Author: demian
 */

#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include <string>
#include <cstring>
#include <cstdio>
#include <stdexcept>

using std::string;

/**
 * A class responsible for an object of a given type allocated in a
 * shared-between-processes memory.
 * @param T The type of the shared object. The type must be primitive or a
 *          struct of primitives types.
 */
template <typename T>
class SharedMemory
{
public:

	/** The runtime error type thrown by SharedMemory. */
	class Error: std::runtime_error
	{
	public:
		Error(const string& msj, int errorCode = 0):
			runtime_error(msj + " - Error: " +
					(errorCode ? strerror(errorCode) : ""))
		{ }
	};


	/**
	 * Creates a shared memory segment containing an object of type T.
	 * The memory of the object is zero-initialized.
	 * The shared memory is attached to the calling process.
	 *
	 * @param pathName A path to an existing file
	 * @param id       A char to identify the shared memory object.
	 * @param freeOnExit Whether the physical shared memory segment should be
	 *                   freed when the SharedMemory is destroyed.
	 */
	SharedMemory(const string& pathName, char id, bool freeOnExit = true)
	{
		_freeOnExit = freeOnExit;

		// Creates key.
		key_t key = ftok(pathName.c_str(), id);
		if (key == (key_t)-1)
			throw Error("SharedMemory(): Could not create key", errno);

		// Allocates a shared memory segment.
		_shmId = shmget(key, sizeof(T), 0644 | IPC_CREAT);
		if (_shmId == -1)
			throw Error("SharedMemory(): "
					"Could not allocate shared memory", errno);

		// Attaches the shared memory to some address.
		void* shmatRes = shmat(_shmId, NULL, 0);
		if (shmatRes == (void*)-1)
			throw Error("SharedMemory(): "
					"Could not attach shared memory", errno);

		_data = (T*) shmatRes;
	}


	/**
	 * Detaches the shared memory from the calling process and, in case
	 * _freeOnExit is true, the physical shared memory is destroyed.
	 */
	~SharedMemory()
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

	/** Returns a reference to the shared object. */
	const T& get() const { return *_data; }
	      T& get()       { return *_data; }

private:
	bool _freeOnExit;
	T*   _data;
	int  _shmId;

	// Non copiable.
	SharedMemory(SharedMemory&);
	void operator=(SharedMemory&);
};


#endif /* SHARED_MEMORY_H_ */
