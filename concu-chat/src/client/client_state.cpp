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
		_client.sendRegisterNameRequest(userName);
		_client.changeState(new WaitingRegisterNameResponseState(_client,
				userName));
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

const string IdleState::PEER_TABLE_COMMAND = "PEERTABLE";
const string IdleState::START_CHAT_COMMAND = "CHAT";

IdleState::IdleState(Client& client, const string& userName) :
	ConnectedState(client, userName)
{
	_client.getView().showIdleStateCommands();
	_client.sendPeerTableRequest();
}

void IdleState::processUserInputMessage(const string& userInput)
{
	string trimmedInput = trim(userInput);
	if (trimmedInput == PEER_TABLE_COMMAND)
	{
		_client.sendPeerTableRequest();
	}
	else if (trimmedInput.find(START_CHAT_COMMAND) == 0)
	{
		string peerName = trim(trimmedInput.substr(START_CHAT_COMMAND.size()));

		const Peer* peer = _peerTable.getByName(peerName);
		if (peer)
		{
			_client.sendStartChatRequest(peer->getId());
			_client.changeState(new WaitingPeerStartChatResponseState(_client,
					_userName, *peer));
		}
		else
		{
			_client.getView().showInvalidPeerName(peer->getName());
		}
	}
	else
	{
		_client .getView().showInvalidCommand(trimmedInput);
	}
}

void IdleState::processPeerTableResponse(const ByteArray& data)
{
	_peerTable.deserialize(data);
	_client.getView().showPeerTable(_peerTable);
}

void IdleState::processStartChatRequest(const Peer& peer)
{
	_client.changeState(new WaitingUserStartChatResponse(_client, _userName,
			peer));
}

WaitingPeerStartChatResponseState::WaitingPeerStartChatResponseState(
		Client& client, const string& userName, const Peer& peer) :
	ConnectedState(client, userName)
{

}

WaitingUserStartChatResponse::WaitingUserStartChatResponse(Client& client,
		const string& userName, const Peer& peer) :
	ConnectedState(client, userName)
{

}

