/*
 * server.cpp
 *
 *  Created on: 26/06/2010
 *      Author: nicolas
 */

#include "server.h"
#include "exception.h"
#include "ipc/message_queue.h"
#include "constants.h"
#include "core/byte_array.h"

#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#include <sstream>

using std::ostringstream;

Server::Server()
{
	ostringstream oss;
	oss << "queues/server" << getpid() << ".queue";
	_queueFileName = oss.str();
}

Server::~Server()
{

}

int Server::Server(int argc, char** argv)
{
	createQueueFile();

	MessageQueue queue(_queueFileName, CommonConstants::QUEUE_ID, true);
	// TODO analizar como salir del server.
	bool exit = false;

	while (!exit)
	{
		Message message;
		message.deserialize(queue.receiveByteArray());
		processMessage(message);
	}

	destroyQueueFile();
}

void Server::createQueueFile()
{
	if (mknod(_queueFileName.c_str(), 0666, 0) == -1)
		throw Exception("could not create file " + _queueFileName);
}

void Server::destroyQueueFile()
{
	bool unlinkError = unlink(_queueFileName.c_str()) == -1;

	if (unlinkError && errno != ENOENT) // ENOENT = No such file.
		throw Exception("could not destroy file " + _queueFileName);
}

void Server::processMessage(const Message& message)
{
	ByteArray data = message.getData();
	switch (message.getType())
	{

	}
}
