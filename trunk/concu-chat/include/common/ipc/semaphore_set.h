/*
 * semaphore.h
 *
 *  Created on: Apr 16, 2010
 *      Author: demian
 */

#ifndef SEMAPHORE_SET_H_
#define SEMAPHORE_SET_H_

#include "resource.h"
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
class SemaphoreSet: public Resource
{
public:

	/** Structure used to indicate the initial values of the semaphore set. */
	typedef vector<unsigned short> InitValues;

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
	 * @param ownResources Whether this SemaphoreSet "owns" the external
	 *                 system resources associated with it. If true, the system
	 *                 resources will be eliminated on SemaphoreSet's
	 *                 destruction.
	 */
	SemaphoreSet(const string& pathName, char id, size_t nSems, int initVals[],
			bool ownResources);
	SemaphoreSet(const string& pathName, char id, const InitValues& initVals,
			bool ownResources);


	/**
	 * If ownWxternalResources was set to true in construction, destroys the
	 * associated external resources associated with the semaphore set.
	 */
	~SemaphoreSet() throw ();


	/**
	 * If semIndex-th semaphore's value is greater than zero, decrements the
	 * value by 1 and returns immediately.
	 * If semIndex-th semaphore's value is zero, the calling process enters the
	 * semaphore's waiting queue and waits until the semaphore is signal()ed
	 * enough times.
	 */
	void wait(size_t semIndex);


	/**
	 * If there are none processes waiting for the semIndex-th semaphore on the
	 * set, it's value is incremented by 1. Otherwise, the first process in the
	 * waiting queue is waken up and the value of the semaphore remains zero.
	 * Either way, this operation returns immediately.
	 */
	void signal(size_t semIndex);


	/**
	 * Tries to perform a wait() operation on the semIndex-th semaphore of the
	 * set.
	 * The wait() is performed only if it can be done immediately (i.e: the
	 * semaphore's value is greater than zero). Otherwise, no wait() is
	 * performed, and the method returns immediately too.
	 *
	 * @return Whether the wait() was performed (i.e: the semaphore count of the
	 * semIndex-th semaphore could be decremented by one) or not
	 */
	bool tryWait(size_t semIndex);


	/**
	 * @return A proxy to the semIndex-th semaphore in the set.
	 */
	SemaphoreProxy getSemaphore(size_t semIndex);

	virtual void print(ostream& stream) const;

protected:
	virtual void doDispose() throw ();

private:
	int    _semId;
	size_t _nSems;

	void validateIndex(size_t semIndex);

	DECLARE_NON_COPIABLE(SemaphoreSet)
};


/** A simple proxy for individual semaphores. */
class SemaphoreProxy
{
public:
	/** Default copy constructor and assignment operator */

	/** @see SemaphoreSet::wait() */
	void wait() { _set.wait(_index); }

	/** @see SemaphoreSet::signal() */
	void signal() { _set.signal(_index); }

	/** @see SemaphoreSet::tryWait() */
	bool tryWait() { return _set.tryWait(_index); }

private:
	SemaphoreSet& _set;
	int           _index;

	friend class SemaphoreSet;
	SemaphoreProxy(SemaphoreSet& set, int index): _set(set), _index(index) { }
};


#endif /* SEMAPHORE_H_ */
