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

class SemaphoreSet
{
public:

	typedef std::vector<int> IntVector;

	SemaphoreSet(const string& pathName, char id, size_t nSems, int initVals[]);
	SemaphoreSet(const string& pathName, char id, const IntVector& initVals);
	~SemaphoreSet();

	void wait(size_t semIndex);
	void signal(size_t semIndex);
	void tryWait(size_t semIndex);


private:
	int _semId;

	DECLARE_NON_COPIABLE(SemaphoreSet);
};

#endif /* SEMAPHORE_H_ */
