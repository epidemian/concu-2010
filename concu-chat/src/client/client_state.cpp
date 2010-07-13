/*
 * client_state.cpp
 *
 *  Created on: Jun 26, 2010
 *      Author: demian
 */

#include "client_state.h"
#include "client.h"
#include "exception.h"
#include "core/byte_array.h"
#include "utils.h"
#include "model/queue_utils.h"
#include "common.h"

#include <string>
#include <algorithm>
#include <iostream>

using std::cout;
using std::cerr;
using std::string;

class NotSupportedMethodException: public Exception
{
public:
	NotSupportedMethodException() :
		Exception("Not supported method")
	{
	}
};

const string& ClientState::getName()
{
	return _stateName;
}

ClientState::ClientState(const string& stateName, Client& client) :
	_client(client), _stateName(stateName)
{
}

ClientState::~ClientState()
{
}

void ClientState::entryAction()
{
}

void ClientState::exitAction()
{
}

void ClientState::processUserInputMessage(const string& userInput)
{
	cerr << "Invalid processUserInputMessage() call. userInput= " << userInput
			<< "\n";
}

void ClientState::processRegisterNameResponse(bool responseOk)
{
	cerr << "Invalid processRegisterNameResponse() call. responseOk="
			<< std::boolalpha << responseOk << "\n";
}

void ClientState::processPeerTableResponse(const ByteArray&)
{
	cerr << "Invalid processPeerTableResponse() call\n";
}

void ClientState::processStartChatRequest(const Peer& peer)
{
	cerr << "Invalid processStartChatRequest() call. peerName=" << peer << "\n";
}

void ClientState::processStartChatResponse(bool responseOk)
{
	cerr << "Invalid processStartChatResponse() call. responseOk="
			<< responseOk << "\n";
}

void ClientState::processEndChat()
{
	cerr << "Invalid processEndChat() call.\n";
}

void ClientState::processChatMessage(const string& chatMessage)
{
	cerr << "Invalid processChatMessage() call. chatMessage=" << chatMessage
			<< "\n";
}

void ClientState::processUserExit()
{
}

NotRegisteredState::NotRegisteredState(Client& client) :
	ClientState("Not registered", client)
{
}

void NotRegisteredState::entryAction()
{
	_client.getView().askUserName();
}

void NotRegisteredState::processUserInputMessage(const string& userInput)
{
	string userName = trim(userInput);
	bool valid = std::count_if(userName.begin(), userName.end(), isalnum) > 0;
	if (!valid)
	{
		_client.getView().showInvalidName(userName);
	}
	else if (_client.sendRegisterNameRequest(userName))
	{
		_client.changeState(new WaitingRegisterNameResponseState(_client,
				userName));
	}
}

WaitingRegisterNameResponseState::WaitingRegisterNameResponseState(
		Client& client, const string& userName) :
	ClientState("Waiting register name response", client), _userName(userName)
{
}

void WaitingRegisterNameResponseState::processRegisterNameResponse(
		bool responseOk)
{
	if (responseOk)
		_client.changeState(new IdleState(_client, _userName));
	else
	{
		_client.getView().showAlreadyUsedName(_userName);
		_client.changeState(new NotRegisteredState(_client));
	}
}

RegisteredState::RegisteredState(const string& stateName, Client& client,
		const string& userName) :
	ClientState(stateName, client), _userName(userName)
{
}

void RegisteredState::processStartChatRequest(const Peer& peer)
{
	_client.sendStartChatResponse(peer, false);
}

void RegisteredState::processUserExit()
{
	_client.sendUnregisterNameRequest(_userName);
}

IdleState::IdleState(Client& client, const string& userName) :
	RegisteredState("Idle", client, userName)
{
}

void IdleState::entryAction()
{
	_client.getView().showIdleStateCommands();
	_client.sendPeerTableRequest();
}

void IdleState::processUserInputMessage(const string& userInput)
{
	string trimmedInput = trim(userInput);
	if (trimmedInput == ClientView::PEER_TABLE_COMMAND)
	{
		_client.sendPeerTableRequest();
	}
	else if (trimmedInput.find(ClientView::START_CHAT_COMMAND) == 0)
	{
		string peerName = trim(trimmedInput.substr(
				ClientView::START_CHAT_COMMAND.size()));
		processStartChatCommand(peerName);
	}
	else
	{
		_client.getView().showInvalidCommand(trimmedInput);
	}
}

void IdleState::processStartChatCommand(const string& peerName)
{
	if (peerName == _userName)
	{
		_client.getView().showCannotChatWithYourself();
	}
	else
	{
		const Peer* peer = _peerTable.getByName(peerName);
		if (peer && _client.sendStartChatRequest(*peer, _userName))
			_client.changeState(new WaitingPeerStartChatResponseState(_client,
					_userName, *peer));
		else
			_client.getView().showInvalidPeerName(peerName);
	}
}

void IdleState::processPeerTableResponse(const ByteArray& data)
{
	_peerTable.deserialize(data);
	_client.getView().showPeerTable(_peerTable);
}

void IdleState::processStartChatRequest(const Peer& peer)
{
	_client.changeState(new WaitingUserStartChatResponseState(_client,
			_userName, peer));
}

WaitingPeerStartChatResponseState::WaitingPeerStartChatResponseState(
		Client& client, const string& userName, const Peer& peer) :
	RegisteredState("Waiting for " + peer.getName() + " start chat response",
			client, userName), _peer(peer)
{
}

void WaitingPeerStartChatResponseState::entryAction()
{
	_client.getView().showWaitingPeerResponse(_peer.getName());
}

void WaitingPeerStartChatResponseState::processUserInputMessage(const string&)
{
	// TODO: tratar el cancelar (?)
	_client.getView().showWaitingPeerResponse(_peer.getName());
}

void WaitingPeerStartChatResponseState::processStartChatResponse(
		bool responseOk)
{
	if (responseOk)
	{
		_client.getView().showPeerAcceptedChat(_peer.getName());
		_client.changeState(new ChattingState(_client, _userName, _peer));
	}
	else
	{
		_client.getView().showPeerCanceledChat(_peer.getName());
		_client.changeState(new IdleState(_client, _userName));
	}
}

WaitingUserStartChatResponseState::WaitingUserStartChatResponseState(
		Client& client, const string& userName, const Peer& peer) :
	RegisteredState("Waiting for user start chat response", client, userName),
			_peer(peer)
{
}

void WaitingUserStartChatResponseState::entryAction()
{
	_client.getView().askUserStartChatWith(_peer.getName());
}

void WaitingUserStartChatResponseState::processUserInputMessage(
		const string& userInput)
{
	string trimmedInput = trim(userInput);
	bool userSayYes = _client.getView().isYesString(trimmedInput);
	bool userSayNo = _client.getView().isNoString(trimmedInput);

	if (userSayYes || userSayNo)
	{
		bool startChatting = userSayYes;
		bool couldSendResponse = _client.sendStartChatResponse(_peer,
				startChatting);

		if (startChatting && couldSendResponse)
			_client.changeState(new ChattingState(_client, _userName, _peer));
		else
			_client.changeState(new IdleState(_client, _userName));
	}
	else
	{
		_client.getView().askUserStartChatWith(_peer.getName());
	}
}

void WaitingUserStartChatResponseState::processUserExit()
{
	_client.sendStartChatResponse(_peer, false);
}

ChattingState::ChattingState(Client& client, const string& userName,
		const Peer& peer) :
	RegisteredState("Chatting with " + peer.getName(), client, userName),
			_peer(peer), _peerQueue(getClientQueueFileName(peer.getId()),
					getQueueId(), false)
{
}

void ChattingState::entryAction()
{
	_client.getView().showStartChatSession(_peer.getName());
}

void ChattingState::exitAction()
{
	_client.getView().showEndChatSession(_peer.getName());
}

void ChattingState::processUserInputMessage(const string& userInput)
{
	bool endChat = false;
	if (trim(userInput) == ClientView::END_CHAT_COMMAND)
	{
		_client.sendEndChatMessage(_peerQueue);
		endChat = true;
	}
	else
	{
		endChat = !_client.sendChatMessage(_peerQueue, userInput);
	}

	if (endChat)
		_client.changeState(new IdleState(_client, _userName));
}

void ChattingState::processEndChat()
{
	_client.getView().showPeerLeftChat(_peer.getName());
	_client.changeState(new IdleState(_client, _userName));
}

void ChattingState::processChatMessage(const string& chatMessage)
{
	_client.getView().showChatMessage(_peer.getName(), chatMessage);
}

void ChattingState::processUserExit()
{
	RegisteredState::processUserExit();
	_client.sendEndChatMessage(_peerQueue);
}

