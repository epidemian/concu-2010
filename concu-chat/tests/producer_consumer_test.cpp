/*
 * producer_consumer_test.cpp
 *
 *  Created on: Apr 19, 2010
 *      Author: demian
 */

#include "ipc/mutex_set.h"
#include "ipc/semaphore_set.h"
#include "ipc/shared_memory.h"
#include "random.h"
#include "exception.h"

#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sstream>

using std::string;
using std::cout;
using std::ostringstream;

const size_t MAX_BUFFER_SIZE = 10;

const char SEMAPHORE_ID  = 'G';
const char SHARED_MEM_ID = 'o';
const char MUTEX_ID      = 'd';

const size_t ITEMS_READY_IND = 0;
const size_t EMPTY_PLACES_IND = 1;
const size_t MUTEX_IND = 0;


struct Item
{
	static const size_t DESC_SIZE = 256;
	char description[DESC_SIZE];
};

/** A global variable for exiting loops. */
sig_atomic_t exitLoop;

struct Buffer
{
	Buffer(): _size(0) { }

	void push(Item& item)
	{
		if (_size == MAX_BUFFER_SIZE)
			throw std::logic_error("Buffer::push(): buffer full");
		_items[_size++] = item;
	}

	Item pop()
	{
		if (_size == 0)
			throw std::logic_error("Buffer::pop(): buffer full");
		return _items[--_size];
	}

private:
	Item _items[MAX_BUFFER_SIZE];
	size_t  _size;
};


int producerProcess(const string& pathName, int producerNo);
int consumerProcess(const string& pathName, int consumerNo);
void initializeSharedReources(const string& pathName);
void finalizeSharedReources(const string& pathName);
bool launchProcesses(const string& pathName, size_t nProducers,
		size_t nConsumers, vector<pid_t>& childPids, int& childRet);
void waitForChildren(vector<pid_t>& childPids);
void initializeSignalHandlers();

int main(int argc, char* argv[])
{
	exitLoop = false;
	initializeSignalHandlers();

	cout << "Semaphore Test\n\n";

	string pathName = argv[0];

	size_t nProducers, nConsumers;
	switch(argc)
	{
	case 3 :
		nProducers = strToInt(argv[2]);
		nConsumers = strToInt(argv[2]);
		break;
	case 1 :
		nProducers = randomInt(10) + 1;
		nConsumers = randomInt(10) + 1;
		break;
	default :
		cout << "Usage: " << argv[0] << " <n-producers> <n-consumers>\n";
		return 1;
	}

	cout << "Using " << nProducers << " producers and "
	     << nConsumers << " consumers\n"
	     << "Parent pid: " << getpid() << "\n"
	     << "Starting in 2 seconds\n";
	sleep(2);

	try {
		initializeSharedReources(pathName);

		vector<pid_t> childPids;

		int childRet;
		if (launchProcesses(pathName, nProducers, nConsumers, childPids,
				childRet))
		{
			// Here die the children.
			return childRet;
		}
		else {
			// This is the parent of all children.
			waitForChildren(childPids);
			finalizeSharedReources(pathName);
			return 0;
		}
	}
	catch(...)
	{
		throw;
	}
}

void whateverHandler(int signum)
{
	cout << "Process " << getpid() <<  " received signal " << signum << "\n";
	exitLoop = true;
}


void interruptionHandler(int signum)
{
	cout << "Process " << getpid() << " received signal " << signum
		 << "!\n";
	exitLoop = true;
}


void initializeSignalHandlers()
{
	struct sigaction sa;
	sa.sa_handler = interruptionHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, 0);
	sigaction(SIGTERM, &sa, 0);
}


SemaphoreSet::InitValues getSemaphoreInitValues()
{
	SemaphoreSet::InitValues values(2);
	values[ITEMS_READY_IND] = 0; // No items ready.
	values[EMPTY_PLACES_IND] = MAX_BUFFER_SIZE;
	return values;
}


void initializeSharedReources(const string& pathName)
{
	// Creates shared memory, semaphores and mutex.
	SharedMemory<Buffer> buffer(pathName, SHARED_MEM_ID, true);
	buffer.get() = Buffer(); // The buffer gets constructed with 0 items.

	// It's not completely necessary to initialize semaphores, but it's cool
	// that, as this ensures that this is the process that will initialize them,
	// the others may use different initialization values (which is wrong) and
	// they will be ignored.
	SemaphoreSet semaphores(pathName, SEMAPHORE_ID, getSemaphoreInitValues(),
			true);
	MutexSet mutex(pathName, MUTEX_ID, 1, true);

	// If a resource could not be created and raised an exception, the other
	// resources already created would have been freed.
	// But now change the ownership:
	buffer.setOwnResources(false);
	semaphores.setOwnResources(false);
	mutex.setOwnResources(false);

	// Now when the objects destroy, they will not deallocate the resources.
}

void finalizeSharedReources(const string& pathName)
{
	SharedMemory<Buffer> buffer(pathName, SHARED_MEM_ID, true);
	SemaphoreSet semaphores(pathName, SEMAPHORE_ID, getSemaphoreInitValues(),
			true);
	MutexSet mutex(pathName, MUTEX_ID, 1, true);
}

bool launchProcesses(const string& pathName, size_t nProducers,
		size_t nConsumers, vector<pid_t>& childPids, int& childRet)
{
	childPids.clear();

	for (size_t i = 0, n = nProducers + nConsumers; i < n; ++i)
	{
		pid_t pid = fork();
		switch (pid)
		{
		case -1 :
			throw Exception(string("fork()") + strerror(errno));
		case 0  :
			// Child.
			childRet = i < nProducers ? producerProcess(pathName, i)
					: consumerProcess(pathName, i - nProducers);
			return true;
		default :
			// Parent.
			childPids.push_back(pid);
			break;
		}
	}
	// Only parent reaches here.
	return false;
}


void waitForChildren(vector<pid_t>& childPids)
{
	for (size_t i = 0; i < childPids.size(); ++i)
	{
		if (waitpid(childPids[i], NULL, 0) == -1)
			throw Exception(string("waitpid()") + strerror(errno));
	}
}

Item produce(int producerNo);
void consume(Item& item, int consumerNo);

int producerProcess(const string& pathName, int producerNo)
{
	cout << "Producer " << producerNo << " started\n";

	// Resource initialization.
	SharedMemory<Buffer> sharedBuf(pathName, SHARED_MEM_ID, false);
	SemaphoreSet semaphores(pathName, SEMAPHORE_ID, getSemaphoreInitValues(),
			false);
	MutexSet mutexSet(pathName, MUTEX_ID, 1, false);

	Buffer& buffer = sharedBuf.get();
	SemaphoreProxy itemsReady = semaphores.getSemaphore(ITEMS_READY_IND);
	SemaphoreProxy emptyPlaces = semaphores.getSemaphore(EMPTY_PLACES_IND);
	MutexProxy mutex = mutexSet.getMutex(0);

	while (!exitLoop)
	{
		Item item = produce(producerNo);
		emptyPlaces.wait();

		// Critical section.
		mutex.lock();
		buffer.push(item);
		mutex.unlock();

		itemsReady.signal();
	}
	cout << "Producer " << producerNo << " (process " << getpid()
	     << ") is exiting gracefully\n";
	return 0;
}


int consumerProcess(const string& pathName, int consumerNo)
{
	cout << "Consumer " << consumerNo << " started\n";

	// Resource initialization.
	SharedMemory<Buffer> sharedBuf(pathName, SHARED_MEM_ID, false);
	SemaphoreSet semaphores(pathName, SEMAPHORE_ID, getSemaphoreInitValues(),
			false);
	MutexSet mutexSet(pathName, MUTEX_ID, 1, false);

	Buffer& buffer = sharedBuf.get();
	SemaphoreProxy itemsReady = semaphores.getSemaphore(ITEMS_READY_IND);
	SemaphoreProxy emptyPlaces = semaphores.getSemaphore(EMPTY_PLACES_IND);
	MutexProxy mutex = mutexSet.getMutex(0);

	while (!exitLoop)
	{
		itemsReady.wait();

		// Critical section.
		mutex.lock();
		Item item = buffer.pop();
		mutex.unlock();

		emptyPlaces.signal();
		consume(item, consumerNo);
	}
	cout << "Consumer " << consumerNo << " (process " << getpid()
	     << ") is exiting gracefully\n";
	return 0;
}

Item produce(int producerNo)
{
	static const char* types[] = { "Alfajores", "Bananas", "Melones",
			"Choripanes", "Empanadas", "Pizzas", "Chocolates" };
	static const int nTypes = ARR_SIZE(types);

	int randNo = randomInt(100) + 1;
	const char* randType = types[randomInt(nTypes)];

	ostringstream description;
	description << randNo << " " << randType;
	Item item;
	strncpy(item.description, description.str().c_str(),
			Item::DESC_SIZE);

	cout << "Producer " << producerNo << " made " << item.description << "\n";
	cout << std::flush;
	return item;
}


void consume(Item& item, int consumerNo)
{
	cout << "Consumer " << consumerNo << " uses " << item.description << "\n";
}
