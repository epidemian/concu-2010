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
#include "constants.h"

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

ClientState::ClientState(Client& client) :
	_client(client)
{

}

ClientState::~ClientState()
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

void ClientState::processExit()
{
}

NotConnectedState::NotConnectedState(Client& client) :
	ClientState(client)
{
	_client.getView().askUserName();
}

NotConnectedState::~NotConnectedState()
{
}

void NotConnectedState::processUserInputMessage(const string& userInput)
{
	string userName = trim(userInput);
	bool valid = std::count_if(userName.begin(), userName.end(), isalnum) > 0;
	if (valid)
	{
		if (_client.sendRegisterNameRequest(userName))
			_client.changeState(new WaitingRegisterNameResponseState(_client,
					userName));
		else
		{
			_client.getView().showCouldNotContactServer();
		}

	}
	else
	{
		_client.getView().showInvalidName(userName);
	}
}

WaitingRegisterNameResponseState::WaitingRegisterNameResponseState(
		Client& client, const string& userName) :
	ClientState(client), _userName(userName)
{
}

WaitingRegisterNameResponseState::~WaitingRegisterNameResponseState()
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
		_client.changeState(new NotConnectedState(_client));
	}
}

ConnectedState::ConnectedState(Client& client, const string& userName) :
	ClientState(client), _userName(userName)
{
}

void ConnectedState::processExit()
{
	_client.sendUnregisterNameRequest(_userName);
}

IdleState::IdleState(Client& client, const string& userName) :
	ConnectedState(client, userName)
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
		_client .getView().showInvalidCommand(trimmedInput);
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
		if (peer)
		{
			_client.sendStartChatRequest(peer->getId(), _userName);
			_client.changeState(new WaitingPeerStartChatResponseState(_client,
					_userName, *peer));
		}
		else
		{
			_client.getView().showInvalidPeerName(peerName);
		}
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
	ConnectedState(client, userName), _peer(peer)
{
	_client.getView().showWaitingPeerResponse(peer.getName());
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
	ConnectedState(client, userName), _peer(peer)
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
		_client.sendStartChatResponse(_peer.getId(), startChatting);

		if (startChatting)
			_client.changeState(new ChattingState(_client, _userName, _peer));
		else
			_client.changeState(new IdleState(_client, _userName));
	}
	else
	{
		_client.getView().askUserStartChatWith(_peer.getName());
	}
}

ChattingState::ChattingState(Client& client, const string& userName,
		const Peer& peer) :
	ConnectedState(client, userName), _peer(peer), _peerQueue(
			getClientQueueFileName(peer.getId()), CommonConstants::QUEUE_ID,
			false)
{
	_client.getView().showStartChatMessage(_peer.getName());
}

void ChattingState::processUserInputMessage(const string& userInput)
{
	if (trim(userInput) == ClientView::END_CHAT_COMMAND)
	{
		_client.sendEndChatMessage(_peerQueue);
		_client.changeState(new IdleState(_client, _userName));
	}
	else
	{
		_client.sendChatMessage(_peerQueue, userInput);
	}
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

void ChattingState::processStartChatRequest(const Peer& peer)
{
	_client.sendStartChatResponse(peer.getId(), false);
}

