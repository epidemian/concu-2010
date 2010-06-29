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
	void sendRegisterNameRequest(const string& userName);
	void sendPeerTableRequest();
	void sendStartChatRequest(pid_t peerId);
	void sendStartChatResponse(pid_t peerId, bool responseOk);
	void sendChatMessage(MessageQueue& peerQueue, const string& chatMessage);
	void sendEndChatMessage(MessageQueue& peerQueue);

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

	void sendMessage(const string& queueFileName, MessageType type, const ByteArray& data = ByteArray());
	void sendMessageToServer(MessageType type, const ByteArray& data = ByteArray());
	void sendMessageToPeer(pid_t peerId, MessageType type, const ByteArray& data = ByteArray());
	void sendMessageToMyself(MessageType type, const ByteArray& data = ByteArray());
};

#endif /* CLIENT_H_ */
