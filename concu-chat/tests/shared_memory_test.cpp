/*
 * shared_memory_test.cpp
 *
 *  Created on: Apr 8, 2010
 *      Author: demian
 */

#include "tests.h"

#include "ipc/shared_memory.h"
#include "person.h"

#include <unistd.h>
#include <sys/wait.h>

#include <string>
#include <cstdlib>
#include <iostream>

using std::string;
using std::cout;

const char INT_ID = 'i';
const char PERSON_ID = 'p';

int writerProcess(pid_t childPid, const string& pathName);
int readerProcess(const string& pathName);

int main(int argc, char* argv[])
{
	cout << "Shared Memory Test\n\n";

	string pathName = argv[0];

	pid_t pid = fork();
	return pid ? writerProcess(pid, pathName)
			   : readerProcess(pathName);
}


int writerProcess(pid_t childPid, const string& pathName)
{
	SharedMemory<int>    sharedInt   (pathName, INT_ID);
	SharedMemory<Person> sharedPerson(pathName, PERSON_ID);

	sharedInt.get() = 481516;
	Person& person = sharedPerson.get(); // A reference to the shared person.
	person = Person("Jesus", 2010);

	cout << "Writer: created shared int = " << sharedInt.get() << "\n";
	cout << "Writer: created person with name = " << person.name()
	     << " and age = " << person.age() << "\n";

	// "Waits" for reader to read first values.
	sleep(4);

	person.becomeOlder();
	cout << "Writer: Happy birthday " << person.name() << "!\n";

	// Waits for reader.
	waitpid(childPid, NULL, 0);

	cout << "Writer: I'm finished\n";
	return 0;
}


int readerProcess(const string& pathName)
{
	SharedMemory<int>    sharedInt   (pathName, INT_ID);
	SharedMemory<Person> sharedPerson(pathName, PERSON_ID);
	Person& person = sharedPerson.get();

	// "Waits" for writer to write first values.
	sleep(2);

	cout << "Reader: read shared int = " << sharedInt.get() << "\n";
	cout << "Reader: read shared person with name = " << person.name()
	     << " and age = " << person.age() << "\n";

	// "Waits" for second value change.
	sleep(6);

	cout << "Reader: now " << person.name() << " is " << person.age() << "!\n";

	cout << "Reader: I'm finished\n";
	return 0;
}
