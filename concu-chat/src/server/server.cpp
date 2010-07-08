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
#include "model/queue_utils.h"
#include "model/model_error.h"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

using std::cout;
using std::cerr;

namespace
{

void showServerStartMessage()
{
	cout << "Servicio de Localización iniciado. Usar el comando "
			<< "'server-admin --quit' o una señal SIGINT (crtl + C) para salir\n";
}

void showServerExitMessage()
{
	cout << "Cerrando Servicio de Localización...\n";
}

void signalHandler(int)
{
	showServerExitMessage();
	exit(EXIT_SUCCESS);
}

} // end namespace


Server::Server(int argc, char** argv)
{
}

Server::~Server()
{
}

int Server::run()
{
	createQueueFile();
	atexit(destroyQueueFile);

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	MessageQueue queue(getServerQueueFileName(), CommonConstants::QUEUE_ID, true);
	bool exit = false;

	showServerStartMessage();

	while (!exit)
	{
		Message message;
		message.deserialize(queue.receiveByteArray());
		processMessage(message, exit);
	}

	showServerExitMessage();
	return EXIT_SUCCESS;
}

void Server::createQueueFile()
{
	if (mknod(getServerQueueFileName().c_str(), 0666, 0) == -1)
		throw Exception("could not create file " + getServerQueueFileName());
}

void Server::destroyQueueFile()
{
	bool unlinkError = unlink(getServerQueueFileName().c_str()) == -1;

	if (unlinkError && errno != ENOENT) // ENOENT = No such file.
		throw Exception("could not destroy file " + getServerQueueFileName());
}

void Server::processMessage(const Message& message, bool& exit)
{
	ByteArrayReader reader(message.getData());

	switch (message.getType())
	{
	case Message::TYPE_PEER_TABLE_REQUEST:
	{
		pid_t userPid = message.getMessengerPid();

		processPeerTableRequest(userPid);
		break;
	}
	case Message::TYPE_REGISTER_NAME_REQUEST:
	{
		string userName = reader.readString();
		pid_t userPid = message.getMessengerPid();

		registerNameRequest(userName, userPid);
		break;
	}
	case Message::TYPE_UNREGISTER_NAME_REQUEST:
	{
		string userName = reader.readString();

		unregisterNameRequest(userName);
		break;
	}
	case Message::TYPE_SHOW_PEER_TABLE_REQUEST:
	{
		showPeerTable();
		break;
	}
	case Message::TYPE_SERVER_EXIT:
	{
		exit = true;
		break;
	}
	default:
		throw Exception("Invalid message type " + toStr(message.getType()));
	}
}

void Server::registerNameRequest(string userName, pid_t userPid)
{
	bool registerOk = !_peerTable.containsName(userName);
	if (registerOk)
	{
		Peer peer(userName, userPid);
		_peerTable.add(peer);
	}

	string userQueueFileName = getClientQueueFileName(userPid);
	MessageQueue queue(userQueueFileName, CommonConstants::QUEUE_ID, false);

	ByteArrayWriter writer;
	writer.write(registerOk);

	Message message(Message::TYPE_REGISTER_NAME_RESPONSE, getpid(),
			writer.getByteArray());
	queue.sendByteArray(message.serialize());
}

void Server::processPeerTableRequest(pid_t userPid)
{
	string userQueueFileName = getClientQueueFileName(userPid);
	MessageQueue queue(userQueueFileName, CommonConstants::QUEUE_ID, false);

	Message message(Message::TYPE_PEER_TABLE_RESPONSE, getpid(),
			_peerTable.serialize());
	queue.sendByteArray(message.serialize());
}

void Server::unregisterNameRequest(string userName)
{
	try
	{
		_peerTable.remove(userName);
		cout << "The user has been removed\n";
	} catch (ModelError& e)
	{
		cerr << "Couldn't remove. The user doesn't exit\n";
	}
}

void Server::showPeerTable()
{
	cout << _peerTable;
}
