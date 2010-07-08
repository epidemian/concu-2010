/*
 * client_view.h
 *
 *  Created on: Jun 26, 2010
 *      Author: demian
 */

#ifndef CLIENT_VIEW_H_
#define CLIENT_VIEW_H_

#include "model/peer_table.h"
#include "utils.h"

#include <string>
using std::string;

class ClientView
{
private:
	static const string LOWERCASE_YES;
	static const string LOWERCASE_NO;

public:
	static const string EXIT_COMMAND;
	static const string PEER_TABLE_COMMAND;
	static const string START_CHAT_COMMAND;
	static const string END_CHAT_COMMAND;

	ClientView();

	void showIgnoredParameters(int argc, char* argv[]);
	void showWelcomeMessage();
	void askUserName();
	void showCouldNotContactServer();
	void showCouldNotContactPeer(const string& peerName);
	void showInvalidName(const string& userName);
	void showAlreadyUsedName(const string& userName);
	void showPeerTable(const PeerTable& peerTable);
	void showIdleStateCommands();
	void showCannotChatWithYourself();
	void showInvalidPeerName(const string& peerName);
	void showInvalidCommand(const string& command);
	void showWaitingPeerResponse(const string& peerName);
	void askUserStartChatWith(const string& peerName);
	void showPeerCanceledChat(const string& peerName);
	void showPeerAcceptedChat(const string& peerName);
	void showStartChatSession(const string& peerName);
	void showEndChatSession(const string& peerName);
	void showPeerLeftChat(const string& peerName);
	void showChatMessage(const string& peerName, const string& chatMessage);

	bool isYesString(const string& str);
	bool isNoString(const string& str);

	DECLARE_NON_COPIABLE(ClientView)
};

#endif /* CLIENT_VIEW_H_ */
