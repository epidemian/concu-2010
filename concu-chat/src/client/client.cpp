/*
 * client.cpp
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#include "client.h"
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

Client::Client(int argc, char* argv[])
{
	ostringstream oss;
	oss << "queues/client" << getpid() << ".queue";
	_queueFileName = oss.str();
}

Client::~Client()
{

}

int Client::run()
{
	createQueueFile();

	pid_t pid = fork();

	switch (pid)
	{
	case -1 :
		throw Exception("fork()");
		break;
	case 0 : // Child.
		runUserInputProcess();
		break;
	default : // Parent.
		runMainProcess();
		break;
	}
	return 0;
}

void Client::runUserInputProcess()
{
	MessageQueue queue(_queueFileName, CommonConstants::QUEUE_ID, false);

	string line;
	while (std::getline(std::cin, line))
	{
		Message message(Message::TYPE_USER_INPUT, stringToByteArray(line));
		queue.sendByteArray(message.serialize());
	}
	Message message(Message::TYPE_USER_EXIT, stringToByteArray(line));
	queue.sendByteArray(message.serialize());
}

void Client::runMainProcess()
{
	MessageQueue queue(_queueFileName, CommonConstants::QUEUE_ID, true);
	bool exit = false;

	// TODO Registrarse en el server.

	while (!exit)
	{
		Message message;
		message.deserialize(queue.receiveByteArray());
		processMessage(message,exit);
	}
}

void Client::createQueueFile()
{
	if (mknod(_queueFileName.c_str(), 0666, 0) == -1)
		throw Exception("could not create file " + _queueFileName);
}

void Client::processMessage(const Message& message, bool& exitNow)
{
	ByteArray data = message.getData();
	switch (message.getType())
	{
	case Message::TYPE_USER_INPUT:
		processUserInputMessage(data);
		break;
	case Message::TYPE_USER_EXIT:
		exitNow = false;
		break;
	}
}

void Client::processUserInputMessage(const ByteArray& data)
{
	string msg = byteArrayToString(data);
	msg.size();
}

