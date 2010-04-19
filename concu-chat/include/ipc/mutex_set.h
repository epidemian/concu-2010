/*
 * mutex_set.h
 *
 *  Created on: Apr 19, 2010
 *      Author: demian
 */

#ifndef MUTEX_SET_H_
#define MUTEX_SET_H_

#include "semaphore_set.h"

class MutexProxy;

class MutexSet
{
public:
	MutexSet(const string& pathName, char id, size_t nMutex,
			bool ownExternalResources) :
		_semSet(pathName, id, SemaphoreSet::InitValues(nMutex, 1),
				ownExternalResources)
	{ }

	void lock   (size_t mutexIndex) { _semSet.wait          (mutexIndex); }
	void unlock (size_t mutexIndex) { _semSet.signal        (mutexIndex); }
	bool tryLock(size_t mutexIndex) { return _semSet.tryWait(mutexIndex); }


	/**
	 * @return A proxy to the mutexIndex'th mutex in the set.
	 */
	MutexProxy getMutex(size_t mutexIndex);

private:
	SemaphoreSet _semSet;

	DECLARE_NON_COPIABLE(MutexSet)
};


/**
 * A class with two X's in it's name... must be a very sexy class!
 */
class MutexProxy
{
public:
	/** Default copy constructor and assignment operator */

	/** @see MutexSet::lock() */
	void lock() { _proxy.wait(); }

	/** @see MutexSet::unlock() */
	void unlock() { _proxy.signal(); }

	/** @see MutexSet::tryLock() */
	bool tryLock() { return _proxy.tryWait(); }

private:
	SemaphoreProxy& _proxy;

	friend class MutexSet;
	MutexProxy(SemaphoreProxy& proxy): _proxy(proxy) { }
};

inline MutexProxy MutexSet::getMutex(size_t mutexIndex)
{
	return MutexProxy(_semSet.getSemaphore(mutexIndex));
}


#endif /* MUTEX_SET_H_ */
