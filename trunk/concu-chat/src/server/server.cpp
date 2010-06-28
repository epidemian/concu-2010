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

Server::Server(int argc, char** argv) {
	ostringstream oss;
	oss << "queues/server" << getpid() << ".queue";
	_queueFileName = oss.str();
}

Server::~Server() {

}

int Server::run() {
	createQueueFile();

	MessageQueue queue(_queueFileName, CommonConstants::QUEUE_ID, true);
	// TODO analizar como salir del server.
	bool exit = false;

	while (!exit) {
		Message message;
		message.deserialize(queue.receiveByteArray());
		processMessage(message);
	}

	destroyQueueFile();

	return 0;
}

void Server::createQueueFile() {
	if (mknod(_queueFileName.c_str(), 0666, 0) == -1)
		throw Exception("could not create file " + _queueFileName);
}

void Server::destroyQueueFile() {
	bool unlinkError = unlink(_queueFileName.c_str()) == -1;

	if (unlinkError && errno != ENOENT) // ENOENT = No such file.
		throw Exception("could not destroy file " + _queueFileName);
}

string Server::getUserQueueFileName(pid_t pid)
{
	ostringstream oss;
	oss << "queues/client" << pid << ".queue";
	return oss.str();
}

void Server::processMessage(const Message& message) {
	ByteArrayReader reader(message.getData());

	switch (message.getType()) {
	case Message::TYPE_PEER_TABLE_REQUEST: {
		pid_t userPid = message.getMessengerPid();

		processPeerTableRequest(userPid);
		break;
	}
	case Message::TYPE_REGISTER_NAME_REQUEST: {
		string userName = reader.readString();
		pid_t userPid = message.getMessengerPid();

		registerNameRequest(userName, userPid);
		break;
	}
	default:
		throw Exception("Invalid message type " + toStr(message.getType()));
	}
}

void Server::registerNameRequest(string userName, pid_t userPid)
{
	bool response = _peerTable.containsName(userName);
	if (!response) {
		Peer peer(userName, userPid);
		_peerTable.add(peer);
	}

	string userQueueFileName = this->getUserQueueFileName(userPid);
	MessageQueue queue(userQueueFileName, CommonConstants::QUEUE_ID, false);

	ByteArrayWriter writer;
	writer.write(response);

	Message message(Message::TYPE_REGISTER_NAME_RESPONSE, getpid(), writer.getByteArray());
	queue.sendByteArray(message.serialize());
}

void Server::processPeerTableRequest(pid_t userPid)
{
	string userQueueFileName = this->getUserQueueFileName(userPid);
	MessageQueue queue(userQueueFileName, CommonConstants::QUEUE_ID, false);

	Message message(Message::TYPE_PEER_TABLE_RESPONSE, getpid(), _peerTable.serialize());
	queue.sendByteArray(message.serialize());
}
