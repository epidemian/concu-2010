/*
 * client_view.cpp
 *
 *  Created on: Jun 26, 2010
 *      Author: demian
 */

#include "client_view.h"
#include "client_state.h"
#include "utils.h"

#include <iostream>
#include <sstream>

namespace
{

template<typename T> void print(const T& t)
{
	std::cout << t << std::flush;
}

template<typename T> void println(const T& t)
{
	std::cout << t << std::endl;
}

void println()
{
	println("");
}

void printLineSeparator()
{
	println("========================================"
		"========================================");
}

} // end namespace

const string ClientView::EXIT_COMMAND = "EXIT";
const string ClientView::PEER_TABLE_COMMAND = "PEERTABLE";
const string ClientView::START_CHAT_COMMAND = "CHAT";
const string ClientView::END_CHAT_COMMAND = "ENDCHAT";
const string ClientView::LOWERCASE_YES = "yes";
const string ClientView::LOWERCASE_NO = "no";

ClientView::ClientView()
{
}

void ClientView::showIgnoredParameters(int argc, char* argv[])
{
	if (argc > 1)
	{
		print("Ignoring parameters: ");
		for (int i = 1; i < argc; i++)
			print(toStr(argv[i]) + " ");
		println();
	}
}

void ClientView::showWelcomeMessage()
{
	printLineSeparator();
	println("Welcome to Concu-Chat 2010!");
	println("Type " + EXIT_COMMAND
			+ " or enter an EOF character (ctrl + D) to exit at any moment.");
	printLineSeparator();
}

void ClientView::askUserName()
{
	print("Enter your name: ");
}

void ClientView::showCouldNotContactServer()
{
	println("Could not contact server =(");
}

void ClientView::showCouldNotContactPeer(const string& peerName)
{
	println("Could not contact " + peerName + " =(");
}

void ClientView::showInvalidName(const string& userName)
{
	println("The name \"" + userName + "\" is not valid");
}

void ClientView::showAlreadyUsedName(const string& userName)
{
	println("The name \"" + userName
			+ "\" is already in use. Try another one =P");
}

void ClientView::showPeerTable(const PeerTable& peerTable)
{
	print(peerTable);
}

void ClientView::showIdleStateCommands()
{
	println("Commands:");
	println(PEER_TABLE_COMMAND + " to show the connected peers");
	println(START_CHAT_COMMAND + " <peer-name> to start chatting with a peer");
}

void ClientView::showCannotChatWithYourself()
{
	println("You cannot chat with youself! (sorry)");
}

void ClientView::showInvalidPeerName(const string& peerName)
{
	println("Peer " + peerName + " does not exist. Try updating the peer table");
}

void ClientView::showInvalidCommand(const string& command)
{
	println("Invalid command: " + command);
}

void ClientView::showWaitingPeerResponse(const string& peerName)
{
	println("Waiting for " + peerName + " response");
}

void ClientView::askUserStartChatWith(const string& peerName)
{
	println(peerName + " wants to chat with you");
	print("Accept? (" + LOWERCASE_YES + "/" + LOWERCASE_NO + "): ");
}

void ClientView::showPeerCanceledChat(const string& peerName)
{
	println(peerName + " is busy now. Sorry =(");
}

void ClientView::showPeerAcceptedChat(const string& peerName)
{
	println(peerName + " accepted!");
}

void ClientView::showStartChatSession(const string& peerName)
{
	printLineSeparator();
	println("Chat session with " + peerName + " started");
	println("Type " + END_CHAT_COMMAND + " to end the chat session");
	printLineSeparator();
}

void ClientView::showEndChatSession(const string& peerName)
{
	println("Chat session with " + peerName + " ended");
	printLineSeparator();
}

void ClientView::showPeerLeftChat(const string& peerName)
{
	println(peerName + " has left =(");
}

void ClientView::showChatMessage(const string& peerName,
		const string& chatMessage)
{
	println(peerName + " says: " + chatMessage);
}

bool ClientView::isYesString(const string& str)
{
	return toLowerCase(str) == LOWERCASE_YES;
}

bool ClientView::isNoString(const string& str)
{
	return toLowerCase(str) == LOWERCASE_NO;
}

