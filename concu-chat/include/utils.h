/*
 * utils.h
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdexcept>

#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define DECLARE_NON_COPIABLE(className) private: \
	className(className&); \
	void operator = (className&);

int strToInt(const char* str) throw (std::invalid_argument);

#endif /* UTILS_H_ */
