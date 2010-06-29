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
#include "client_state.h"

#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#include <string>
#include <sstream>
#include <iostream>

using std::ostringstream;
using std::cout;
using std::string;

const string Client::PEER_TABLE_COMMAND = "PEERTABLE";
const string Client::START_CHAT_COMMAND = "CHAT";


void showIgnoredParameters(int argc, char* argv[])
{
	if (argc > 1)
	{
		cout << "Ignorig parameters: ";
		for (int i = 1; i < argc; i++)
			cout << argv[i] << " ";
		cout << "\n";
	}
}

Client::Client(int argc, char* argv[]) :
	_state(0)
{
	ostringstream oss;
	oss << "queues/client" << getpid() << ".queue";
	_queueFileName = oss.str();

	showIgnoredParameters(argc, argv);
}

Client::~Client()
{
	changeState(0);
}

int Client::run()
{
	createQueueFile();

	pid_t pid = fork();

	switch (pid)
	{
	case -1:
		throw Exception("fork()");
		break;
	case 0: // Child.
		runUserInputProcess();
		break;
	default: // Parent.
		changeState(new NotConnectedState(*this));
		runMainProcess();
		destroyQueueFile();
		break;
	}
	return 0;
}

void Client::changeState(ClientState* newState)
{
	if (_state)
		delete _state;
	_state = newState;
}

void Client::sendRegisterNameRequest(const string& userName)
{
	ByteArrayWriter writer;
	writer.writeString(userName);
	ByteArray data = writer.getByteArray();
	Message msg = Message(Message::TYPE_REGISTER_NAME_REQUEST, getpid(), data);

	sendMessageToServer(msg);
}

ClientView& Client::getView()
{
	return _view;
}

void Client::runUserInputProcess()
{
	MessageQueue queue(_queueFileName, CommonConstants::QUEUE_ID, false);

	string line;
	while (std::getline(std::cin, line))
	{
		ByteArrayWriter writer;
		writer.writeString(line);
		Message message(Message::TYPE_USER_INPUT, getpid(),
				writer.getByteArray());
		queue.sendByteArray(message.serialize());
	}
	Message message(Message::TYPE_USER_EXIT, getpid(), stringToByteArray(line));
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
		processMessage(message, exit);
	}
}

void Client::createQueueFile()
{
	if (mknod(_queueFileName.c_str(), 0666, 0) == -1)
		throw Exception("could not create file " + _queueFileName);
}

void Client::destroyQueueFile()
{
	bool unlinkError = unlink(_queueFileName.c_str()) == -1;

	if (unlinkError && errno != ENOENT) // ENOENT = no such file.
		throw Exception("destroyQueueFile(): could not unlink the queue file "
				+ _queueFileName);
}

void Client::processMessage(const Message& message, bool& exitNow)
{
	ByteArrayReader reader(message.getData());

	switch (message.getType())
	{
	case Message::TYPE_USER_INPUT:
	{
		string userInput = reader.readString();
		_state->processUserInputMessage(userInput);
		break;
	}
	case Message::TYPE_USER_EXIT:
	{
		exitNow = true;
		break;
	}
	case Message::TYPE_REGISTER_NAME_RESPONSE:
	{
		bool responseOk = reader.read<bool> ();
		_state->processRegisterNameResponse(responseOk);
		break;
	}
	case Message::TYPE_PEER_TABLE_RESPONSE:
	{
		_state->processPeerTableResponse(message.getData());
		break;
	}
	default:
		throw Exception("Invalid message type " + toStr(message.getType()));
	}
}

void Client::sendMessageToServer(const Message& msg)
{
	// TODO: Implement me!
}

