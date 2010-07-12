/*
 * client.cpp
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#include "client.h"
#include "exception.h"
#include "ipc/message_queue.h"
#include "model/queue_utils.h"
#include "constants.h"
#include "core/byte_array.h"
#include "client_state.h"
#include "ipc/ipc_error.h"
#include "logger.h"

#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

#include <string>
#include <sstream>
#include <iostream>
#include <cstring>

using std::ostringstream;
using std::string;

namespace
{

void log(const string& message)
{
	ostringstream oss;
	oss << "Client " << getpid() << ": " << message;
	Logger::instance().log(oss.str());
}

} // end namespace


Client::Client() :
	_state(0)
{
	_queueFileName = getClientQueueFileName();
}

Client::~Client()
{
	changeState(0);
}

int Client::run()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	createQueueFile();

	getView().showWelcomeMessage();

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
	return EXIT_SUCCESS;
}

void Client::changeState(ClientState* newState)
{
	if (_state)
		delete _state;
	_state = newState;
}

bool Client::sendRegisterNameRequest(const string& userName)
{
	ByteArrayWriter writer;
	writer.writeString(userName);
	ByteArray data = writer.getByteArray();

	return sendMessageToServer(Message::TYPE_REGISTER_NAME_REQUEST, data);
}

bool Client::sendUnregisterNameRequest(const string& userName)
{
	ByteArrayWriter writer;
	writer.writeString(userName);
	ByteArray data = writer.getByteArray();

	return sendMessageToServer(Message::TYPE_UNREGISTER_NAME_REQUEST, data);
}

bool Client::sendPeerTableRequest()
{
	sendMessageToServer(Message::TYPE_PEER_TABLE_REQUEST);
}

bool Client::sendStartChatRequest(const Peer& peer, const string& userName)
{
	ByteArrayWriter writer;
	writer.writeString(userName);
	ByteArray data = writer.getByteArray();

	return sendMessageToPeer(peer, Message::TYPE_START_CHAT_REQUEST, data);
}

bool Client::sendStartChatResponse(const Peer& peer, bool responseOk)
{
	ByteArrayWriter writer;
	writer.write(responseOk);
	ByteArray data = writer.getByteArray();

	return sendMessageToPeer(peer, Message::TYPE_START_CHAT_RESPONSE, data);
}

bool Client::sendChatMessage(MessageQueue& peerQueue, const string& chatMessage)
{
	ByteArrayWriter writer;
	writer.writeString(chatMessage);
	ByteArray data = writer.getByteArray();

	return sendMessageToQueue(peerQueue, Message::TYPE_CHAT_MESSAGE, data);
}

bool Client::sendEndChatMessage(MessageQueue& peerQueue)
{
	return sendMessageToQueue(peerQueue, Message::TYPE_END_CHAT);
}

ClientView& Client::getView()
{
	return _view;
}

void Client::runUserInputProcess()
{
	MessageQueue queue(_queueFileName, CommonConstants::QUEUE_ID, false);

	string line;
	bool exit = false;

	while (!exit && bool(std::getline(std::cin, line)))
	{
		if (trim(line) == ClientView::EXIT_COMMAND)
		{
			exit = true;
		}
		else
		{
			ByteArrayWriter writer;
			writer.writeString(line);
			ByteArray data = writer.getByteArray();
			sendMessageToQueue(queue, Message::TYPE_USER_INPUT, data);
		}
	}
	sendMessageToQueue(queue, Message::TYPE_USER_EXIT);
}

void Client::runMainProcess()
{
	log("Up and running...");

	MessageQueue queue(_queueFileName, CommonConstants::QUEUE_ID, true);
	bool exit = false;

	while (!exit)
	{
		Message message;
		message.deserialize(queue.receiveByteArray());
		processMessage(message, exit);
	}

	log("Closing down");
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
		_state->processExit();
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
	case Message::TYPE_START_CHAT_REQUEST:
	{
		string peerName = reader.readString();
		Peer peer(peerName, message.getMessengerPid());
		_state->processStartChatRequest(peer);
		break;
	}
	case Message::TYPE_START_CHAT_RESPONSE:
	{
		bool responseOk = reader.read<bool> ();
		_state->processStartChatResponse(responseOk);
		break;
	}
	case Message::TYPE_CHAT_MESSAGE:
	{
		string chatMessage = reader.readString();
		_state->processChatMessage(chatMessage);
		break;
	}
	case Message::TYPE_END_CHAT:
	{
		_state->processEndChat();
		break;
	}
	default:
		throw Exception("Invalid message type " + toStr(message.getType()));
	}
}

bool Client::sendMessageToQueue(MessageQueue& queue, MessageType type,
		const ByteArray& data)
{
	try
	{
		Message message(type, getpid(), data);
		queue.sendByteArray(message.serialize());
		return true;
	} catch (IpcError& e)
	{
		// TODO log
		std::cout
				<< "Could not send message to queue. IpcError thrown. Error code: "
				<< e.getErrorCode() << " - " << strerror(e.getErrorCode())
				<< "\n";
		return false;
	}
}

bool Client::sendMessage(const string& queueFileName, MessageType type,
		const ByteArray& data)
{
	try
	{
		MessageQueue queue(queueFileName, CommonConstants::QUEUE_ID, false);
		return sendMessageToQueue(queue, type, data);
	} catch (IpcError& e)
	{
		// TODO log
		std::cout
				<< "Could not create message queue. IpcError thrown. Error code: "
				<< e.getErrorCode() << " - " << strerror(e.getErrorCode())
				<< "\n";
		return false;
	}
}

bool Client::sendMessageToServer(MessageType type, const ByteArray& data)
{
	bool couldSend = sendMessage(getServerQueueFileName(), type, data);
	if (!couldSend)
		getView().showCouldNotContactServer();
	return couldSend;
}

bool Client::sendMessageToPeer(const Peer& peer, MessageType type,
		const ByteArray& data)
{
	string peerQueueFileName = getClientQueueFileName(peer.getId());
	bool couldSend = sendMessage(peerQueueFileName, type, data);
	if (!couldSend)
		getView().showCouldNotContactPeer(peer.getName());
	return couldSend;
}

