/*
 * queue_utils.h
 *
 *  Created on: 29/06/2010
 *      Author: nicolas
 */

#ifndef QUEUE_UTILS_H_
#define QUEUE_UTILS_H_

#include <string>

#include <sys/types.h>

using std::string;

string getClientQueueFileName(pid_t pid = getpid());
string getServerQueueFileName();

#endif /* QUEUE_UTILS_H_ */
