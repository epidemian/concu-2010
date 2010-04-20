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

RawMessageQueue::RawMessageQueue(const string& pathName, char id){

	_freeOnExit = false;

	// Creates key.
	key_t key = ftok(pathName.c_str(), id);
	if (key == (key_t)-1)
		throw IpcError("RawMessageQueue(): Could not create key", errno);

	// Try to create the message queue.
	_queueId = msgget(key, 0644 | IPC_CREAT | IPC_EXCL);
	if (_queueId == -1)
	{
		// The queue has already been created. We try to connect.
		if (errno == EEXIST)
		{
			_queueId = msgget(key, 0644 | IPC_CREAT);
			if (_queueId == -1)
				throw IpcError("RawMessageQueue(): Could not create or connet"
						" to the message queue", errno);
		}
		else
			throw IpcError("RawMessageQueue(): Could not create or connet"
											" to the message queue", errno);
	}
	else
		// The process that create the queue is the one that has to erase it.
		_freeOnExit = true;
}

void RawMessageQueue::send(const void* buffer, size_t size, long mtype)
{
	// Builds the package to send the message.
	char *writeBuff = (char*)malloc(sizeof(long) + size);
	if (!writeBuff)
		throw IpcError("RawMessageQueue::send(): Could not build the package "
				"to be send", errno);

	*(long*)writeBuff = mtype;

	memcpy(writeBuff+sizeof(long), buffer, size);

	// Sends the message.
	int errorCode = msgsnd(_queueId,writeBuff,size,0);
	if (errorCode == -1)
		throw IpcError("RawMessageQueue::send(): Could not write into the "
				"queue", errno);

	free(writeBuff);
}

void RawMessageQueue::receive(void* buffer, size_t size, long mtype)
{
	// Builds the package to receive the message.
	char *readBuff = (char*)malloc(sizeof(long) + size);
	if (!readBuff)
		throw IpcError("RawMessageQueue::receive(): Could not build the package "
				"to be received", errno);

	// Receives the message.
	int errorCode = msgrcv(_queueId,readBuff,size,mtype,0);
	if (errorCode == -1)
		throw IpcError("RawMessageQueue::receive(): Could not read from the "
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

void MessageQueue::send(const std::string& message, long mtype)
{
	/**
	 * Sends the string.
	 * The second parameter has a "+1" expression because  the size method
	 * returns only the number of characters in the string, not including any
	 * null-termination.
	 */
	RawMessageQueue::send(message.c_str(), message.size()+1, mtype);
}

const std::string MessageQueue::receive(long mtype)
{
	int bufferSize = 1024;
	bool receiveOk = false;
	std::string message;

	while (!receiveOk)
	{
		char* readBuff = (char*)malloc(bufferSize);

		int errorCode = msgrcv(_queueId,readBuff,bufferSize-1,mtype,0);
		if (errorCode == -1)
			// We don't receive everything that we were supposed to receive.
			if (errno == E2BIG)
				bufferSize *= 2;
			// Something wrong happended.
			else
				throw IpcError("RawMessageQueue::receive(): Could not read from"
						" the queue", errno);
		// Everything goes ok!
		else
		{
			message.append(readBuff + sizeof(long));
			receiveOk = true;
		}

		free(readBuff);
	}

	return message;
}
