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
	_queueId = msgget(key, 0644 | IPC_EXCL);
	if (_queueId == -1)
	{
		// The queue doesn't exist. So we try to create one.
		_queueId = msgget(key, 0644 | IPC_CREAT);
		if (_queueId == -1)
			throw IpcError("RawMessageQueue(): Could not create or connet to "
					"the message queue", errno);
		// The process that create the queue is the one that has to erase it.
		_freeOnExit = true;
	}
}

void RawMessageQueue::write(const void* buffer, size_t size, long mtype){

	// Builds the package to send the message.
	char *finalBuff = (char*)malloc(sizeof(long) + size);
	if (!finalBuff)
		throw IpcError("RawMessageQueue::write(): Could not build the package "
				"to be send", errno);

	*(long*)finalBuff = mtype;

	memcpy(finalBuff+sizeof(long), buffer, size);

	// Sends the message.
	int returnValue = msgsnd(_queueId,finalBuff,size,0);
	if (returnValue == -1)
		throw IpcError("RawMessageQueue::write(): Could not write into the "
				"queue", errno);

	free(finalBuff);
}

void RawMessageQueue::read(void* buffer, size_t size, long mtype)
{
	// Builds the package to receive the message.
	char *finalBuff = (char*)malloc(sizeof(long) + size);
	if (!finalBuff)
		throw IpcError("RawMessageQueue::read(): Could not build the package "
				"to be received", errno);

	// Receives the message.
	msgrcv(_queueId,finalBuff,size,mtype,0);

	memcpy(buffer, finalBuff+sizeof(long), size);

	free(finalBuff);
}

RawMessageQueue::~RawMessageQueue(){

	if (_freeOnExit) msgctl(_queueId,IPC_RMID, NULL);
}
