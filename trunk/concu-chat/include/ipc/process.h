/*
 * process.h
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include "utils.h"

class Process
{
public:
	void start();
	void wait();

protected:
	Process();
	virtual ~Process();
	virtual int run() = 0;

private:
	struct ProcessImpl;
	ProcessImpl* _impl;

	DECLARE_NON_COPIABLE(Process)
};


#endif /* PROCESS_H_ */
