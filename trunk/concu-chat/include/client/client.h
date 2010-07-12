/*
 * client.h
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "ipc/message_queue.h"
#include "model/message.h"
#include "client_view.h"

#include <string>

class ClientState;
class Message;

using std::string;

class Client
{
public:

	Client();

	~Client();

	int run();

	void changeState(ClientState* newState);

	// Messages to server.
	bool sendRegisterNameRequest(const string& userName);
	bool sendUnregisterNameRequest(const string& userName);
	bool sendPeerTableRequest();

	// Messages to peer.
	bool sendStartChatRequest(const Peer& peer, const string& userName);
	bool sendStartChatResponse(const Peer& peer, bool responseOk);
	bool sendChatMessage(MessageQueue& peerQueue, const string& chatMessage);
	bool sendEndChatMessage(MessageQueue& peerQueue);

	ClientView& getView();
private:

	typedef Message::MessageType MessageType;

	string _queueFileName;
	ClientState* _state;
	ClientView _view;

	void runUserInputProcess();
	void runMainProcess();

	void createQueueFile();
	void destroyQueueFile();
	void processMessage(const Message& message, bool& exitNow);

	bool sendMessageToQueue(MessageQueue& queue, MessageType type,
			const ByteArray& data = ByteArray());
	bool sendMessage(const string& queueFileName, MessageType type,
			const ByteArray& data = ByteArray());
	bool sendMessageToServer(MessageType type, const ByteArray& data =
			ByteArray());
	bool sendMessageToPeer(const Peer& peer, MessageType type,
			const ByteArray& data = ByteArray());
};

#endif /* CLIENT_H_ */
