/*
 * file_lock_test.cpp
 *
 *  Created on: 07/07/2010
 *      Author: nicolas
 */

#include "exception.h"
#include "ipc/file_lock.h"

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <errno.h>

using std::string;
using std::cout;

void destroyFile(string pathname);

int test1();
int child(int fd);
int father(int fd);

int main(int, char**)
{
	return test1();
}

int test1()
{
	string pathname = "filelock.txt";

	int fd = open(pathname.c_str(), O_RDWR | O_CREAT, 0666);
	if (fd == -1)
		throw Exception("Error in open");

	pid_t pid = fork();
	switch (pid)
	{
	case -1:
		throw Exception("Error in fork");
		return 1;
	case 0:
		// Child code.
		return child(fd);
	default:
		// Parent code.
		father(fd);
	}

	wait(0);

	close(fd);
	destroyFile(pathname);
	return 0;
}

int child(int fd)
{
	// Child acquires the lock of the file.
	{
		FileLock lock(fd);
		cout << "I'm the child and I've locked the file!\n"
				"I'll wait 7 seconds.\n";
		sleep(7);
	}
	cout << "I'm the child and I've released the lock of the file!\n";

	return 0;
}

int father(int fd)
{
	{
		sleep(2);
		cout << "I'm the father and I want to read the file!\n";
		FileLock lock(fd);
		cout << "I'm the father and I've locked the file!\n";
	}
	cout << "I'm the father and I've released the lock of the file!\n";

	return 0;
}

void destroyFile(string pathname)
{
	bool unlinkError = unlink(pathname.c_str()) == -1;

	if (unlinkError && errno != ENOENT) // ENOENT = No such file.
		throw Exception("Could not destroy file " + pathname);
}
