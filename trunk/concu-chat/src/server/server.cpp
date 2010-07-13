/*
 * server.cpp
 *
 *  Created on: 26/06/2010
 *      Author: nicolas
 */

#include "server.h"
#include "exception.h"
#include "ipc/message_queue.h"
#include "core/byte_array.h"
#include "model/queue_utils.h"
#include "model/model_error.h"
#include "logger.h"
#include "common.h"

#include <iostream>
#include <sstream>

#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

using std::cout;
using std::cerr;
using std::ostringstream;

namespace
{

void log(const string& message)
{
	ostringstream oss;
	oss << "Server " << getpid() << ": " << message;
	Logger::instance().log(oss.str());
}

void showServerStartMessage()
{
	cout << "Servicio de Localización iniciado. Usar el comando "
			<< "'server-admin --quit' o una señal SIGINT (crtl + C) para salir\n";
	log("Up and running...");
}

void showServerAlreadyRunning()
{
	cout << "El servicio de localización ya se encuentra corriendo!\n";
	log("Another server already running");
}

void showServerExitMessage()
{
	cout << "Cerrando Servicio de Localización...\n";
	log("Closing down");
}

void signalHandler(int)
{
	showServerExitMessage();
	exit(EXIT_SUCCESS);
}

} // end namespace


Server::Server()
{
}

Server::~Server()
{
}

int Server::run()
{
	if (!createQueueFile())
	{
		showServerAlreadyRunning();
		return EXIT_FAILURE;
	}

	atexit(destroyQueueFile);

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	MessageQueue queue(getServerQueueFileName(), getQueueId(), true);
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

bool Server::createQueueFile()
{
	if (mknod(getServerQueueFileName().c_str(), 0666, 0) == -1)
	{
		if (errno == EEXIST)
			return false;
		else
			throw Exception("could not create file " + getServerQueueFileName());
	}

	return true;
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

		processRegisterNameRequest(userName, userPid);
		break;
	}
	case Message::TYPE_UNREGISTER_NAME_REQUEST:
	{
		string userName = reader.readString();

		processUnregisterNameRequest(userName);
		break;
	}
	case Message::TYPE_SHOW_PEER_TABLE_REQUEST:
	{
		processShowPeerTableRequest();
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

void Server::processRegisterNameRequest(const string& userName, pid_t userPid)
{
	bool registerOk = !_peerTable.containsName(userName);
	if (registerOk)
	{
		Peer peer(userName, userPid);
		_peerTable.add(peer);
	}

	ostringstream logMsg;
	logMsg << "Received register name request from " << userPid << ", name = "
			<< userName << ", register OK = " << std::boolalpha << registerOk;
	log(logMsg.str());

	string userQueueFileName = getClientQueueFileName(userPid);
	MessageQueue queue(userQueueFileName, getQueueId(), false);

	ByteArrayWriter writer;
	writer.write(registerOk);

	Message message(Message::TYPE_REGISTER_NAME_RESPONSE, getpid(),
			writer.getByteArray());
	queue.sendByteArray(message.serialize());
}

void Server::processPeerTableRequest(pid_t userPid)
{
	ostringstream logMsg;
	logMsg << "Received peer table request from " << userPid;
	log(logMsg.str());

	string userQueueFileName = getClientQueueFileName(userPid);
	MessageQueue queue(userQueueFileName, getQueueId(), false);

	Message message(Message::TYPE_PEER_TABLE_RESPONSE, getpid(),
			_peerTable.serialize());
	queue.sendByteArray(message.serialize());
}

void Server::processUnregisterNameRequest(const string& userName)
{
	try
	{
		_peerTable.remove(userName);
		ostringstream logMsg;
		logMsg << "Received unregister name request, name = " << userName;
		log(logMsg.str());
	} catch (ModelError& e)
	{
		cerr << "Couldn't remove. The user doesn't exit\n";
	}
}

void Server::processShowPeerTableRequest()
{
	ostringstream logMsg;
	logMsg << "Received show peer table request. " << _peerTable;
	log(logMsg.str());

	cout << _peerTable;
}
