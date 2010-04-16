/*
 * semaphore.h
 *
 *  Created on: Apr 16, 2010
 *      Author: demian
 */

#ifndef SEMAPHORE_SET_H_
#define SEMAPHORE_SET_H_

#include "utils.h"
#include <sys/sem.h>

#include <vector>
#include <string>

using std::string;
using std::vector;

class SemaphoreProxy;

/**
 * A set of semaphores.
 */
class SemaphoreSet
{
public:

	/** Structure used to indicate the initial values of the semaphore set. */
	typedef vector<unsigned short> SemValues;

	/**
	 * Creates a semaphore set.
	 * If the process is the first one to create a semaphore set with the given
	 * path name and id, a new low-level semaphore set will be created and
	 * initialized with the given values.
	 * If another process has already created a semaphore set with the same
	 * path name and id, this semaphore set will be associated with the same
	 * low-level resources, and those will not be initialized (i.e: the initVals
	 * vector will be ignored.
	 *
	 * @param pathName A path to an existing file.
	 * @param id       An identifier for the semaphore set.
	 * @param nSems    The number of semaphores in the set.
	 * @param initVals The initial values for each semaphore in the set.
	 * @param ownExternalResources Whether this SemaphoreSet "owns" the external
	 *                 system resources associated with it. If true, the system
	 *                 resources will be eliminated on SemaphoreSet's
	 *                 destruction.
	 */
	SemaphoreSet(const string& pathName, char id, size_t nSems, int initVals[],
			bool ownExternalResources);
	SemaphoreSet(const string& pathName, char id, const SemValues& initVals,
			bool ownExternalResources);

	/**
	 * If ownWxternalResources was set to true in construction, destroys the
	 * associated external resources associated with the semaphore set.
	 */
	~SemaphoreSet();

	void wait(size_t semIndex);
	void signal(size_t semIndex);

	/**
	 * @return Whether the semaphore count of the semIndex'th semaphore could be
	 * decremented by one (i.e: was different than zero).
	 */
	bool tryWait(size_t semIndex);

	/**
	 * @return A proxy to the semIndex'th semaphore in the set.
	 */
	SemaphoreProxy getSemaphore(size_t semIndex);

private:
	int    _semId;
	size_t _nSems;
	bool   _ownResources;

	void validateIndex(size_t semIndex);

	DECLARE_NON_COPIABLE(SemaphoreSet)
};


/** A simple proxy for individual semaphores. */
class SemaphoreProxy
{
public:
	/** Default copy constructor and assignment operator */

	void wait() { _set.wait(_index); }
	void signal() { _set.signal(_index); }
	bool tryWait() { return _set.tryWait(_index); }

private:
	SemaphoreSet& _set;
	int           _index;

	friend class SemaphoreSet;
	SemaphoreProxy(SemaphoreSet& set, int index): _set(set), _index(index) { }
};


#endif /* SEMAPHORE_H_ */
