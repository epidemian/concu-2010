/*
 * main.cpp
 *
 *  Created on: 29/06/2010
 *      Author: nicolas
 */

#include <iostream>
#include <string>
#include <unistd.h>

#include "ipc/message_queue.h"
#include "model/queue_utils.h"
#include "model/peer_table.h"

using std::cout;
using std::cerr;
using std::string;

void unregisterUser(MessageQueue& serverQueue, string userName);
void showPeerTable(MessageQueue& serverQueue);
void quit(MessageQueue& serverQueue);

int main(int argc, char **argv)
{
	MessageQueue serverQueue(getServerQueueFileName(), CommonConstants::QUEUE_ID, false);

	while ((c = getopt(argc, argv, "u:m:q")) != -1) {

		switch (c) {
		case 'u':
			unregisterUser(serverQueue, optarg);
			break;
		case 'm':
			showPeerTable(serverQueue);
			break;
		case 'q':
			quit(serverQueue);
			break;
		default:
			cerr << "Incorrect parameter: " << c << "\n";
			return -1;
		}
	}
}

void unregisterUser(MessageQueue& serverQueue, string userName)
{
	/*
	 * Sends the request.
	 */
	ByteArrayWriter writer;
	writer.writeString(userName);

	Message messageSent(Message::TYPE_UNREGISTER_NAME_RESPONSE, getpid(), writer.getByteArray());
	serverQueue.sendByteArray(messageSent.serialize());

	/*
	 * Receives the request.
	 */
	Message messageReceived;
	messageReceived.deserialize(serverQueue.receiveByteArray());

	ByteArrayReader reader(messageReceived.getData());
	bool response = reader.read<bool>();

	cout << "The user " << userName << response ? "has " : "hasn't " << "been removed\n";
}

void showPeerTable(MessageQueue& serverQueue)
{
	/*
	 * Sends the request.
	 */
	Message messageSent(Message::TYPE_PEER_TABLE_REQUEST, getpid());
	serverQueue.sendByteArray(messageSent.serialize());

	/*
	 * Receives the request.
	 */
	Message messageReceived;
	messageReceived.deserialize(serverQueue.receiveByteArray());

	PeerTable peerTable;
	peerTable.deserialize(messageReceived.getData());

	cout << "Peer Table: Name - Adress\n\n";
	PeerTable::PeerTableInterator iterator = peerTable.begin();
	for (; iterator != peerTable.end(); iterator++)
	{
		Peer peer = *iterator;
		cout << peer.getName() << getClientQueueFileName(peer.getId()) << "\n";
	}
}

void quit(MessageQueue& serverQueue)
{
	/*
	 * Sends the request.
	 */
	Message messageSent(Message::TYPE_SERVER_EXIT, getpid());
	serverQueue.sendByteArray(messageSent.serialize());
}
