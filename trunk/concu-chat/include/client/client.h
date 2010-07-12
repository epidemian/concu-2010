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

	/**
	 * Change the current state of the client.
	 */
	void changeState(ClientState* newState);

	/**
	 * Sends a request to register a name to the server.
	 * @param userName The name to be registered.
	 * @return Whether the message could be sent or not.
	 */
	bool sendRegisterNameRequest(const string& userName);

	/**
	 * Sends a request to unregister a name to the server.
	 * @param userName The name to be unregistered.
	 * @return Whether the message could be sent or not.
	 */
	bool sendUnregisterNameRequest(const string& userName);

	/**
	 * Sends a request to the server to get the peer table.
	 * @return Whether the message could be sent or not.
	 */
	bool sendPeerTableRequest();

	/**
	 * Sends a request to start a chat session to a peer.
	 * @param peer The peer whom the user wants to chat.
	 * @param userName The name of the user (this client's user).
	 * @return Whether the message could be sent or not.
	 */
	bool sendStartChatRequest(const Peer& peer, const string& userName);

	/**
	 * Sends a response of a start chat request to a peer.
	 * @param peer The peer that originally sent the start chat request.
	 * @param responseOk True if the user accepted to chat with the peer. False
	 * otherwise.
	 * @return Whether the message could be sent or not.
	 */
	bool sendStartChatResponse(const Peer& peer, bool responseOk);

	/**
	 * Sends a chat message to a peer.
	 * @return Whether the message could be sent or not.
	 */
	bool sendChatMessage(MessageQueue& peerQueue, const string& chatMessage);

	/**
	 * Sends an end chat message to a peer.
	 * @return Whether the message could be sent or not.
	 */
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
