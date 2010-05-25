/*
 * process.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: demian
 */

#include "ipc/process.h"

#include <unistd.h>
#include <sys/wait.h>

#include <cstdlib>

struct Process::ProcessImpl
{
	pid_t pid;
};

Process::Process(): _impl(new ProcessImpl)
{ }

Process::~Process()
{
	delete _impl;
}

void Process::start()
{
	pid_t pid = fork();

	if (pid == -1)
		// tirar excepción..
		;

	else if (pid == 0)
	{
		// Child...
		_impl->pid = getpid();
		exit(this->run());
		// TODO: Hummm... no se si terminar así está bueno, pero tiene que
		// terminar, no puede retornar esta función acá, sería re engorroso
		// usar esta clase así. Pero tampoco se si terminando así con exit()
		// las cosas construidas antes de la llamada a Process:start() se van a
		// destruir bien o van a leakear por todos lados (y con los recursos que
		// vamos a estar manejando, mejor no leakear)...
	}
	else {
		// Parent...
		_impl->pid = pid;
	}
}

void Process::wait()
{
	// TODO: Fijarse de atrapar el retorno del proceso wait()eado.
	waitpid(_impl->pid, NULL, 0);

}
