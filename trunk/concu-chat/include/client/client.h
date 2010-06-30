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

	Client(int argc, char* argv[]);

	~Client();

	int run();

	void changeState(ClientState* newState);

	// Messages to server.
	bool sendRegisterNameRequest(const string& userName);
	bool sendUnregisterNameRequest(const string& userName);
	void sendPeerTableRequest();

	// Messages to peer.
	bool sendStartChatRequest(pid_t peerId, const string& userName);
	bool sendStartChatResponse(pid_t peerId, bool responseOk);
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

	bool sendMessage(const string& queueFileName, MessageType type,
			const ByteArray& data = ByteArray());
	bool sendMessageToServer(MessageType type, const ByteArray& data =
			ByteArray());
	bool sendMessageToPeer(pid_t peerId, MessageType type,
			const ByteArray& data = ByteArray());
	bool sendMessageToMyself(MessageType type, const ByteArray& data =
			ByteArray());
};

#endif /* CLIENT_H_ */
