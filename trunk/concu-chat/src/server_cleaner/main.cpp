/*
 * main.cpp
 *
 *  Created on: 29/06/2010
 *      Author: nicolas
 */

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "exception.h"

#include "ipc/message_queue.h"
#include "model/queue_utils.h"
#include "model/peer_table.h"
#include "model/message.h"
#include "constants.h"
#include "utils.h"
#include "core/byte_array.h"

using std::cout;
using std::cerr;
using std::string;

void unregisterUser(MessageQueue& serverQueue, string userName);
void showPeerTable(MessageQueue& serverQueue);
void quit(MessageQueue& serverQueue);

int main(int argc, char **argv)
{
	MessageQueue serverQueue(getServerQueueFileName(),
			CommonConstants::QUEUE_ID, false);
	int c;
	opterr = 0;

	while ((c = getopt(argc, argv, ":u:mq")) != -1)
	{
		switch (c)
		{
		case 'u':
			unregisterUser(serverQueue, optarg);
			break;
		case 'm':
			showPeerTable(serverQueue);
			break;
		case 'q':
			quit(serverQueue);
			break;
		case ':':
			cerr << "Missing argument " << optopt << "\n";
			return 1;
			break;
		case '?':
			cerr << "Incorrect parameter: " << optopt << "\n";
			return 1;
			break;
		default:
			cerr << "Error parsing arguments\n";
			return 1;
			break;
		}
	}
}

void unregisterUser(MessageQueue& serverQueue, string userName)
{
	ByteArrayWriter writer;
	writer.writeString(userName);

	Message messageSent(Message::TYPE_UNREGISTER_NAME_REQUEST, getpid(),
			writer.getByteArray());
	serverQueue.sendByteArray(messageSent.serialize());
}

void showPeerTable(MessageQueue& serverQueue)
{
	Message messageSent(Message::TYPE_SHOW_PEER_TABLE_REQUEST, getpid());
	serverQueue.sendByteArray(messageSent.serialize());
}

void quit(MessageQueue& serverQueue)
{
	Message messageSent(Message::TYPE_SERVER_EXIT, getpid());
	serverQueue.sendByteArray(messageSent.serialize());
}
