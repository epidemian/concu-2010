/*
 * message_queue.cpp
 *
 *  Created on: 16/04/2010
 *      Author: nicolas
 */

#include "ipc/message_queue.h"
#include "ipc/ipc_error.h"

#include <sys/msg.h>
#include <errno.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <iostream>

RawMessageQueue::RawMessageQueue(const string& pathName, char id){

	_freeOnExit = false;

	// Creates key.
	key_t key = ftok(pathName.c_str(), id);
		if (key == (key_t)-1)
			throw IpcError("RawMessageQueue(): Could not create key", errno);

	// Try to connect to the message queue.
	_queueId = msgget(key, 0644 | IPC_EXCL | IPC_CREAT);
	if (_queueId == -1)
	{
		if (errno == EEXIST)
		{
			// The queue doesn't exist. So we try to create one.
			_queueId = msgget(key, 0644 | IPC_CREAT);
			if (_queueId == -1)
				throw IpcError("RawMessageQueue(): Could not create or connet "
						"to the message queue", errno);
			// The process that create the queue is the one that has to erase it.
			_freeOnExit = true;
		}
		else
			throw IpcError("RawMessageQueue(): Could not create or connet to "
					"the message queue", errno);
	}
}

void RawMessageQueue::write(const void* buffer, size_t size, long mtype)
{
	// Builds the package to send the message.
	char *writeBuff = (char*)malloc(sizeof(long) + size);
	if (!writeBuff)
		throw IpcError("RawMessageQueue::write(): Could not build the package "
				"to be send", errno);

	*(long*)writeBuff = mtype;

	memcpy(writeBuff+sizeof(long), buffer, size);

	// Sends the message.
	int errorCode = msgsnd(_queueId,writeBuff,size,0);
	if (errorCode == -1)
		throw IpcError("RawMessageQueue::write(): Could not write into the "
				"queue", errno);

	free(writeBuff);
}

void RawMessageQueue::read(void* buffer, size_t size, long mtype)
{
	// Builds the package to receive the message.
	char *readBuff = (char*)malloc(sizeof(long) + size);
	if (!readBuff)
		throw IpcError("RawMessageQueue::read(): Could not build the package "
				"to be received", errno);

	// Receives the message.
	int errorCode = msgrcv(_queueId,readBuff,size,mtype,0);
	if (errorCode == -1)
		throw IpcError("RawMessageQueue::read(): Could not read from the "
				"queue", errno);

	memcpy(buffer, readBuff+sizeof(long), size);

	free(readBuff);
}

RawMessageQueue::~RawMessageQueue(){

	if (_freeOnExit)
	{
		int errorCode = msgctl(_queueId,IPC_RMID, NULL);
		if (errorCode == -1)
			perror("~RawMessageQueue(): Could not destroy message queue");
	}
}
