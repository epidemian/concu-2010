/*
 * shared_memory.h
 *
 *  Created on: Apr 8, 2010
 *      Author: demian
 */

#ifndef IPC_SHARED_MEMORY_H_
#define IPC_SHARED_MEMORY_H_

#include "ipc/resource.h"
#include "utils.h"

#include <string>

using std::string;


/**
 * A class responsible for a segment of shared-between-processes memory.
 */
class RawSharedMemory: public Resource
{
public:
	/**
	 * Creates a zero-initialized shared memory segment of a given size.
	 * The shared memory is attached to the calling process.
	 *
	 * @param size     The size, in bytes, of the shared memory segment.
	 * @param pathName A path to an existing file.
	 * @param id       A char to identify the shared memory object.
	 * @param ownResource Whether the physical shared memory segment
	 *                   should be deallocated when the SharedMemory is
	 *                   destroyed.
	 */
	RawSharedMemory(size_t size, const string& pathName, char id,
			bool ownResource = true);

	/**
	 * Detaches the shared memory from the calling process and, in case
	 * ownResource is true, the physical shared memory is destroyed.
	 */
	~RawSharedMemory() throw ();

	const void* get() const { return _data; }
		  void* get()       { return _data; }

	virtual void print(ostream& stream) const;

protected:
	virtual void doDispose() throw ();

private:
	void* _data;
	int   _shmId;

	// Non copiable.
	DECLARE_NON_COPIABLE(RawSharedMemory)
};


/**
 * A class responsible for an object of a given type allocated in a
 * shared-between-processes memory.
 * @param T The type of the shared object. The type must be primitive or a
 *          struct of primitives types.
 */
template <typename T>
class SharedMemory: public Resource
{
public:

	/**
	 * Creates a shared memory segment containing an object of type T.
	 * The memory of the object is zero-initialized and it's constructor is not
	 * called.
	 * The shared memory is attached to the calling process.
	 *
	 * @param pathName A path to an existing file
	 * @param id       A char to identify the shared memory object.
	 * @param ownResource Whether the physical shared memory segment
	 *                   should be deallocated when the SharedMemory is
	 *                   destroyed.
	 */
	SharedMemory(const string& pathName, char id, bool ownResource):
		Resource(ownResource),
		_sharedMem(sizeof(T), pathName, id, ownResource)
	{ }


	/**
	 * Detaches the shared memory from the calling process and, in case
	 * ownResource is true, the physical shared memory is deallocated.
	 * The destructor of the shared object is never called, and it shouldn't
	 * be necessary, as it should be a primitive object with no resources to
	 * deallocate.
	 */
	~SharedMemory() throw () { }


	/** Returns a reference to the shared object. */
	const T& get() const { return * (T*) _sharedMem.get(); }
	      T& get()       { return * (T*) _sharedMem.get(); }


	virtual bool ownResources() const
	{ return _sharedMem.ownResources(); }

	virtual void setOwnResources(bool value)
	{ _sharedMem.setOwnResources(value); }

	virtual void print(ostream& stream) const
	{
		stream << "shared memory wrapping ";
		_sharedMem.print(stream);
	}

protected:
	virtual void doDispose() throw ()
	{ /* Nothing. _sharedMem is the real resource here. */ }

private:
	RawSharedMemory _sharedMem;

	// Non copiable.
	DECLARE_NON_COPIABLE(SharedMemory)
};


#endif /* SHARED_MEMORY_H_ */
