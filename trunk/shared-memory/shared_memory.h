/*
 * shared_memory.h
 *
 *  Created on: Apr 8, 2010
 *      Author: demian
 */

#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_

#include <string>
#include <stdexcept>

using std::string;

/** The runtime error type thrown by RawSharedMemory and SharedMemory. */
class SharedMemoryError: std::runtime_error
{ public: SharedMemoryError(const string& msj, int errorCode = 0); };


/**
 * A class responsible for a segment of shared-between-processes memory.
 */
class RawSharedMemory
{
public:
	/**
	 * Creates a zero-initialized shared memory segment of a given size.
	 * The shared memory is attached to the calling process.
	 *
	 * @param size     The size, in bytes, of the shared memory segment.
	 * @param pathName A path to an existing file.
	 * @param id       A char to identify the shared memory object.
	 * @param freeOnExit Whether the physical shared memory segment should be
	 *                   freed when the SharedMemory is destroyed.
	 */
	RawSharedMemory(size_t size, const string& pathName, char id,
			bool freeOnExit = true);

	/**
	 * Detaches the shared memory from the calling process and, in case
	 * freeOnExit is true, the physical shared memory is destroyed.
	 */
	~RawSharedMemory();

	const void* get() const { return _data; }
		  void* get()       { return _data; }

private:
	bool  _freeOnExit;
	void* _data;
	int   _shmId;

	// Non copiable.
	RawSharedMemory(RawSharedMemory&);
	void operator=(RawSharedMemory&);
};


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
	SharedMemory(const string& pathName, char id, bool freeOnExit = true):
		_sharedMem(sizeof(T), pathName, id, freeOnExit)
	{ }


	/**
	 * Detaches the shared memory from the calling process and, in case
	 * freeOnExit is true, the physical shared memory is destroyed.
	 */
	~SharedMemory() { }

	/** Returns a reference to the shared object. */
	const T& get() const { return * (T*) _sharedMem.get(); }
	      T& get()       { return * (T*) _sharedMem.get(); }

private:
	RawSharedMemory _sharedMem;

	// Non copiable.
	SharedMemory(SharedMemory&);
	void operator=(SharedMemory&);
};


#endif /* SHARED_MEMORY_H_ */
