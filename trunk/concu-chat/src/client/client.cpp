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
	_queueFileName = getClientQueueFileName();
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

	sendMessageToServer(Message::TYPE_REGISTER_NAME_REQUEST, data);
}

void Client::sendUnregisterNameRequest(const string& userName)
{
	ByteArrayWriter writer;
	writer.writeString(userName);
	ByteArray data = writer.getByteArray();

	sendMessageToServer(Message::TYPE_UNREGISTER_NAME_REQUEST, data);
}

void Client::sendPeerTableRequest()
{
	sendMessageToServer(Message::TYPE_PEER_TABLE_REQUEST);
}

void Client::sendStartChatRequest(pid_t peerId, const string& userName)
{
	ByteArrayWriter writer;
	writer.writeString(userName);
	ByteArray data = writer.getByteArray();

	sendMessageToPeer(peerId, Message::TYPE_START_CHAT_REQUEST, data);
}

void Client::sendStartChatResponse(pid_t peerId, bool responseOk)
{
	ByteArrayWriter writer;
	writer.write(responseOk);
	ByteArray data = writer.getByteArray();

	sendMessageToPeer(peerId, Message::TYPE_START_CHAT_RESPONSE, data);
}

void Client::sendChatMessage(MessageQueue& peerQueue, const string& chatMessage)
{
	ByteArrayWriter writer;
	writer.writeString(chatMessage);
	ByteArray data = writer.getByteArray();

	Message message(Message::TYPE_CHAT_MESSAGE, getpid(), data);
	peerQueue.sendByteArray(message.serialize());
}

void Client::sendEndChatMessage(MessageQueue& peerQueue)
{
	Message message(Message::TYPE_END_CHAT, getpid());
	peerQueue.sendByteArray(message.serialize());
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
		bool responseOk = reader.read<bool>();
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

void Client::sendMessage(const string& queueFileName, MessageType type, const ByteArray& data)
{
	MessageQueue serverQueue(queueFileName, CommonConstants::QUEUE_ID, false);
	Message message(type, getpid(), data);
	serverQueue.sendByteArray(message.serialize());
}

void Client::sendMessageToServer(MessageType type, const ByteArray& data)
{
	sendMessage(getServerQueueFileName(), type, data);
}

void Client::sendMessageToPeer(pid_t peerId, MessageType type, const ByteArray& data)
{
	sendMessage(getClientQueueFileName(peerId), type, data);
}

void Client::sendMessageToMyself(MessageType type, const ByteArray& data)
{
	sendMessageToPeer(getpid(), type, data);
}
