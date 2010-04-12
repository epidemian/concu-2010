#include <iostream>
#include "fifo.h"
#include <unistd.h>
#include <sys/wait.h>
#include "person.h"

using std::string;
using std::cout;

int writerProcess(pid_t);
int readerProcess();

int main(int argc, char **argv) {

	pid_t pid = fork();
	return pid ? writerProcess(pid)
			   : readerProcess();
	return 0;
}

int writerProcess(pid_t childPid)
{
	Fifo fifo("fifo.fif");

	FifoWriter w_fifo(fifo);
	Person person("Mario Bros",55);

	/**
	 * Mario Bros enters into the time-travel-pipe.
	 */

	cout << person.name() << " is traveling through the time-travel-pipe. He "
			"is " << person.age() << " years old.\n";
	sleep(4);

	w_fifo.write(person);

	waitpid(childPid, NULL, 0);

	return 0;
}


int readerProcess()
{
	Fifo fifo("fifo.fif");

	FifoReader r_fifo(fifo);
	Person person;

	/**
	 * Mario Bros arrives into the Bros's World.
	 */

	r_fifo.read(person);

	/**
	 * In the new world he becomeOlder ten times.
	 */

	for(int i=0; i<10; i++)
		person.becomeOlder();

	cout << "He arrives to the new world from the time-travel-pipe.\n";
	cout << "The new age of " << person.name() << " is: " << person.age() << "\n";

	return 0;

}

