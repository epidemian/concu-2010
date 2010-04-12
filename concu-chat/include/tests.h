/*
 * tests.h
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#ifndef TESTS_H_
#define TESTS_H_

typedef int (*TestFunction) (int argc, char* argv[]);

int sharedMemoryTest(int argc, char* argv[]);
int fifoTest        (int argc, char* argv[]);


#endif /* TESTS_H_ */
