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


void producerProcess(const string& pathName, int producerNo);
void consumerProcess(const string& pathName, int consumerNo);
void initializeSharedReources(const string& pathName);
void finalizeSharedReources(const string& pathName);
vector<pid_t> launchProcesses(const string& pathName, size_t nProducers,
		size_t nConsumers);
void waitForChildren(vector<pid_t>& childPids);
void initializeParentSignalHandlers();
void initializeChildSignalHandlers();

int main(int argc, char* argv[])
{
	initializeParentSignalHandlers();

	cout << "Semaphore Test\n\n";

	string pathName = argv[0];

	size_t nProducers, nConsumers;
	switch(argc)
	{
	case 3:
		nProducers = strToInt(argv[1]);
		nConsumers = strToInt(argv[2]);
		break;
	case 1 :
		nProducers = randomInt(1, 10);
		nConsumers = randomInt(1, 10);
		break;
	default :
		cout << "Usage: " << argv[0] << " <n-producers> <n-consumers>\n";
		return 1;
	}

	cout << "Using " << nProducers << " producers and "
	     << nConsumers << " consumers\n"
	     << "Parent pid: " << getpid() << "\n"
	     << "Starting in 5 seconds (^C to start now and ^C to finish once it "
	     << "started)\n";

	sleep(5);

	try {
		initializeSharedReources(pathName);

		vector<pid_t> childPids =
				launchProcesses(pathName, nProducers, nConsumers);
		waitForChildren(childPids);
		finalizeSharedReources(pathName);
		return 0;
	}
	catch(...)
	{
		throw;
	}
}


void parentInterruptionHandler(int signum)
{
	cout << "Parent process (pid " << getpid() << ") received signal "
	     << signum << "! I will keep on waiting :)\n";
}


void initializeParentSignalHandlers()
{
	struct sigaction sa;
	sa.sa_handler = parentInterruptionHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; // waitpid() restarts.

	sigaction(SIGINT, &sa, 0);
	sigaction(SIGTERM, &sa, 0);
}


void childInterruptionHandler(int signum)
{
	cout << "Child process (pid " << getpid() << ") received signal " << signum
		 << "! Exiting now...\n";
	// The resources will be collected by RecourceCollector =)
	exit(EXIT_SUCCESS);
}


void initializeChildSignalHandlers()
{
	struct sigaction sa;
	// Child terminates it's execution on interruptions.
	sa.sa_handler = childInterruptionHandler;
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


struct ProducerConsumerResources
{
	SharedMemory<Buffer> buffer;
	SemaphoreSet semaphores;
	MutexSet mutexes;


	ProducerConsumerResources(const string& pathName, bool ownResources):
		buffer(pathName, SHARED_MEM_ID, ownResources),
		semaphores(pathName, SEMAPHORE_ID, getSemaphoreInitValues(),
				ownResources),
		mutexes(pathName, MUTEX_ID, 1, ownResources)
	{ }

	void setOwnResources(bool value)
	{
		buffer.setOwnResources(value);
		semaphores.setOwnResources(value);
		mutexes.setOwnResources(value);
	}
};


void initializeSharedReources(const string& pathName)
{
	// Creates shared memory, semaphores and mutex.
	ProducerConsumerResources res(pathName, true);
	res.buffer.get() = Buffer(); // The buffer gets constructed with 0 items.

	// If a resource could not be created and raised an exception, the other
	// resources already created would have been freed.
	// But now change the ownership:
	res.setOwnResources(false);

	// Now when the objects destroy, they will not deallocate the resources.
}


void finalizeSharedReources(const string& pathName)
{
	ProducerConsumerResources res(pathName, true);
}


vector<pid_t> launchProcesses(const string& pathName, size_t nProducers,
		size_t nConsumers)
{
	vector<pid_t> childrenPids;

	for (size_t i = 0, n = nProducers + nConsumers; i < n; ++i)
	{
		pid_t pid = fork();
		switch (pid)
		{
		case -1 :
			throw Exception(string("fork():") + strerror(errno));
		case 0  :
			// Child.
			initializeChildSignalHandlers();
			if (i < nProducers) producerProcess(pathName, i);
			else consumerProcess(pathName, i - nProducers);
			// Children never reaches here, by just to be sure they end and
			// don't continue with the loop...
			throw
				Exception("Child process continued unexpectedly");
		default :
			// Parent.
			childrenPids.push_back(pid);
			break;
		}
	}
	// Only parent reaches here.
	return childrenPids;
}


void waitForChildren(vector<pid_t>& childPids)
{
	for (size_t i = 0; i < childPids.size(); ++i)
	{
		if (waitpid(childPids[i], NULL, 0) == -1)
			throw Exception(string("waitpid(): ") + strerror(errno));
	}
}

Item produce(int producerNo);
void consume(Item& item, int consumerNo);

void producerProcess(const string& pathName, int producerNo)
{
	cout << "Producer " << producerNo << " started in process " << getpid()
	     << "\n";

	// Resource initialization.
	ProducerConsumerResources res(pathName, false);

	Buffer& buffer             = res.buffer.get();
	SemaphoreProxy itemsReady  = res.semaphores.getSemaphore(ITEMS_READY_IND);
	SemaphoreProxy emptyPlaces = res.semaphores.getSemaphore(EMPTY_PLACES_IND);
	MutexProxy mutex           = res.mutexes.getMutex(0);

	while (true)
	{
		Item item = produce(producerNo);
		emptyPlaces.wait();

		// Critical section.
		mutex.lock();
		buffer.push(item);
		mutex.unlock();

		itemsReady.signal();
	}
}


void consumerProcess(const string& pathName, int consumerNo)
{
	cout << "Consumer " << consumerNo << " started in process " << getpid()
	     << "\n";

	// Resource initialization.
	ProducerConsumerResources res(pathName, false);

	Buffer& buffer             = res.buffer.get();
	SemaphoreProxy itemsReady  = res.semaphores.getSemaphore(ITEMS_READY_IND);
	SemaphoreProxy emptyPlaces = res.semaphores.getSemaphore(EMPTY_PLACES_IND);
	MutexProxy mutex           = res.mutexes.getMutex(0);

	while (true)
	{
		itemsReady.wait();

		// Critical section.
		mutex.lock();
		Item item = buffer.pop();
		mutex.unlock();

		emptyPlaces.signal();
		consume(item, consumerNo);
	}
}

Item produce(int producerNo)
{
	static const char* types[] = { "Alfajores", "Bananas", "Melones",
			"Choripanes", "Empanadas", "Pizzas", "Chocolates" };
	static const int nTypes = ARR_SIZE(types);

	int randNo = randomInt(1, 100);
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
