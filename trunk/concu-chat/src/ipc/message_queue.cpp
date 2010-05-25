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

RawMessageQueue::RawMessageQueue(const string& pathName, char id,
		bool ownResource)
	:Resource(ownResource)
{

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

void RawMessageQueue::sendFixedSize(const void* buffer, size_t size, long mtype)
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

void RawMessageQueue::receiveFixedSize(void* buffer, size_t size, long mtype)
{
	this->tryReceive(buffer,size,mtype);
}

size_t RawMessageQueue::tryReceive(void* buffer, size_t size, long mtype)
{
	// Builds the package to receive the message.
	char *readBuff = (char*)malloc(sizeof(long) + size);
	if (!readBuff)
		throw IpcError("RawMessageQueue::receive(): Could not build the "
				"package to be received", errno);

	// Receives the message.
	ssize_t returnValue = msgrcv(_queueId,readBuff,size,mtype,0);
	// E2BIG: We don't receive everything that we were supposed to receive.
	// Something wrong happened.
	if (returnValue == -1 && errno != E2BIG)
	{
		free(readBuff);
		throw IpcError("RawMessageQueue::receive(): Could not read from"
					   " the queue", errno);
	}

	memcpy(buffer, readBuff + sizeof(long), size);

	free(readBuff);
	return returnValue;
}

RawMessageQueue::~RawMessageQueue() throw()
{
	doDispose();
}

void RawMessageQueue::doDispose() throw ()
{
	if (ownResources())
	{
		int errorCode = msgctl(_queueId,IPC_RMID, NULL);
		if (errorCode == -1)
			perror("~RawMessageQueue(): Could not destroy message queue");
	}
}

void RawMessageQueue::print(ostream& stream) const
{
	stream << "message queue";
}

void MessageQueue::sendByteArray(const ByteArray& message, long mtype)
{
	RawMessageQueue::sendFixedSize(message.data(), message.size(), mtype);
}

const ByteArray MessageQueue::receiveByteArray(long mtype)
{
	int bufferSize = QUEUE_INITAL_BUFFER_SIZE;
	bool receiveOk = false;
	ByteArray message;

	while (!receiveOk)
	{
		char* readBuff = (char*)malloc(bufferSize);

		ssize_t returnValue;

		try{
			returnValue = tryReceive(readBuff,bufferSize,mtype);
		}
		catch(...){
			free(readBuff);
			throw;
		}

		if (returnValue == -1)
		{
			bufferSize *= 2;
		}
		// Everything goes ok!
		else
		{
			message = toByteArray(readBuff,returnValue);
			receiveOk = true;
		}

		free(readBuff);
	}

	return message;
}

void MessageQueue::sendString(const std::string& message, long mtype)
{
	/**
	 * Sends the string.
	 * The second parameter has a "+1" expression because  the size method
	 * returns only the number of characters in the string, not including any
	 * null-termination.
	 */
	RawMessageQueue::sendFixedSize(message.c_str(), message.size()+1, mtype);
}

const std::string MessageQueue::receiveString(long mtype)
{
	ByteArray data = this->receiveByteArray(mtype);
	return getStringFromByteArray(data,0,data.size());
}

