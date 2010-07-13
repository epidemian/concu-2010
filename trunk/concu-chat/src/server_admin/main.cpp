/*
 * main.cpp
 *
 *  Created on: 29/06/2010
 *      Author: nicolas
 */

#include "exception.h"
#include "ipc/message_queue.h"
#include "model/queue_utils.h"
#include "model/peer_table.h"
#include "model/message.h"
#include "utils.h"
#include "core/byte_array.h"
#include "common.h"

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <cstdlib>
#include <getopt.h>
#include <cstring>

using std::cout;
using std::cerr;
using std::string;

void unregisterUser(MessageQueue& serverQueue, string userName);
void showPeerTable(MessageQueue& serverQueue);
void quit(MessageQueue& serverQueue);
void showHelp(const string& commandName);

int main(int argc, char **argv)
{
	string commandName = basename(argv[0]);

	if (argc == 1)
	{
		showHelp(commandName);
		return EXIT_SUCCESS;
	}

	MessageQueue serverQueue(getServerQueueFileName(),
			getQueueId(), false);

	int c;
	opterr = 0;
	struct option longOptions[] =
	{
		{ "help",       no_argument,       0, 'h' },
		{ "unregister", required_argument, 0, 'u' },
		{ "show-peers", no_argument,       0, 's' },
		{ "quit",       no_argument,       0, 'q' },
		{ 0, 0, 0, 0 }
	};

	while ((c = getopt_long(argc, argv, ":hu:sq", longOptions, NULL)) != -1)
	{
		switch (c)
		{
		case 'h':
			showHelp(commandName);
			break;
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
			cerr << "Missing argument for option " << char(optopt) << "\n";
			showHelp(commandName);
			return EXIT_FAILURE;
		case '?':
			cerr << "Invalid option: ";
			if (optopt != 0)
				cerr << char(optopt) << "\n";
			else
				cerr << argv[optind - 1] << "\n";
			showHelp(commandName);
			return EXIT_FAILURE;
		default:
			cerr << "Error parsing arguments\n";
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

void showHelp(const string& commandName)
{
	cout << "Usage: " << commandName << " [option]\n";
	cout << "\n";
	cout << "Server administration utility.\n";
	cout << "\n";
	cout << "Options:\n";
	cout << "    -h, --help                  Show this help message and exit\n";
	cout << "    -u, --unregister <USERNAME> Unregister USERNAME user\n";
	cout << "    -s, --show-peers            Show peer table at server\n";
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
