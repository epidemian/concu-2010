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

class MutexSet: public Resource
{
public:
	MutexSet(const string& pathName, char id, size_t nMutex,
			bool ownResources):
		Resource(ownResources),
		_semSet(pathName, id, SemaphoreSet::InitValues(nMutex, 1),
				ownResources)
	{ }

	void lock   (size_t mutexIndex) { _semSet.wait          (mutexIndex); }
	void unlock (size_t mutexIndex) { _semSet.signal        (mutexIndex); }
	bool tryLock(size_t mutexIndex) { return _semSet.tryWait(mutexIndex); }


	/**
	 * @return A proxy to the mutexIndex'th mutex in the set.
	 */
	MutexProxy getMutex(size_t mutexIndex);

	virtual bool ownResources() const
	{ return _semSet.ownResources(); }

	virtual void setOwnResources(bool value)
	{ _semSet.setOwnResources(value); }

	virtual void print(ostream& stream) const
	{
		stream << "mutex set wrapping ";
		_semSet.print(stream);
	}

protected:

	virtual void doDispose() throw ()
	{ /* Nothing. _semSet is the real resource here. */ }

private:
	SemaphoreSet _semSet;

	DECLARE_NON_COPIABLE(MutexSet)
};


/**
 * A class with two X's in it's name, must be a very sexy class!
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
	SemaphoreProxy _proxy;

	friend class MutexSet;
	MutexProxy(SemaphoreProxy& proxy): _proxy(proxy) { }
};

inline MutexProxy MutexSet::getMutex(size_t mutexIndex)
{
	SemaphoreProxy sem = _semSet.getSemaphore(mutexIndex);
	return MutexProxy(sem);
}


#endif /* MUTEX_SET_H_ */
