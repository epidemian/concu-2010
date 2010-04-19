/*
 * semaphore-test.cpp
 *
 *  Created on: Apr 19, 2010
 *      Author: demian
 */

#include "ipc/shared_memory.h"
#include "ipc/semaphore_set.h"
#include "person.h"

#include <unistd.h>
#include <sys/wait.h>

#include <string>
#include <cstdlib>
#include <iostream>

using std::string;
using std::cout;

const char SEMAPHORE_ID  = 's';
const char SHARED_MEM_ID = 'm';

int writerProcess(pid_t childPid, const string& pathName);
int readerProcess(const string& pathName);

int main(int argc, char* argv[])
{
	cout << "Semaphore Test\n\n";

	string pathName = argv[0];

	pid_t pid = fork();
	return pid ? writerProcess(pid, pathName)
			   : readerProcess(pathName);
}


int writerProcess(pid_t childPid, const string& pathName)
{
	SharedMemory<Person> sharedPerson(pathName, SHARED_MEM_ID, true);
	int initValue = 0;
	SemaphoreSet semSet(pathName, SEMAPHORE_ID, 1, &initValue, true); // true =
	// own resources.
	SemaphoreProxy semaphore = semSet.getSemaphore(0);

	Person& person = sharedPerson.get(); // A reference to the shared person.
	person = Person("John Locke", 48);

	cout << "Writer: created person with name = " << person.name()
	     << " and age = " << person.age() << "\n";

	cout << "Writer: now I'll wait 2 secs and the reader won't do anything.\n";
	sleep(2);

	semaphore.signal();

	// Waits for reader.
	waitpid(childPid, NULL, 0);

	cout << "Writer: I'm finished\n";
	return 0;
}


int readerProcess(const string& pathName)
{
	SharedMemory<Person> sharedPerson(pathName, SHARED_MEM_ID, true);
	Person& person = sharedPerson.get();

	int initValue = 0;
	SemaphoreSet semSet(pathName, SEMAPHORE_ID, 1, &initValue, false); // false
	// = not own resources.
	SemaphoreProxy semaphore = semSet.getSemaphore(0);

	cout << "Reader: I want to read!\n";

	semaphore.wait();

	cout << "Reader: read shared person with name = " << person.name()
	     << " and age = " << person.age() << "\n";

	cout << "Reader: I'm finished\n";
	return 0;
}
