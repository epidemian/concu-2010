/*
 * message_queue.h
 *
 *  Created on: 16/04/2010
 *      Author: nicolas
 */

#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include "utils.h"

#include <string>

using std::string;

class RawMessageQueue
{
public:

	/**
	 * Connect to a message queue, or create it if it doesn't exist
	 *
	 * @param pathName     A path to an existing file.
	 * @param id       A char to identify the shared memory object.
	 */
	explicit RawMessageQueue(const string& pathName, char id);

	/**
	 * Sends size bytes from the buffer to the queue. The id of the message
	 * is mtype. If another process wants to read this message, it has to use
	 * this mtype value.
	 *
	 * @param buffer	Message to be written on the queue.
	 * @param size		The size, in bytes, of the data to be read.
	 * @param mtype		The id of the message.
	 */
	void send(const void* buffer, size_t size, long mtype);

	/**
	 * Receives size bytes from the queue and stores them in the buffer.
	 *
	 * @param buffer	Destination of the message.
	 * @param size		The size, in bytes, of the message to be read.
	 * @param mtype		Zero    : Retrieves the next message on the queue,
	 * 							  regardless of its mtype.
	 * 					Positive: Gets the next message with an mtype equal to
	 * 							  the specified mtype.
	 * 					Negative: Retrieve the first message on the queue whose
	 * 							  mtype field is less than or equal to the
	 * 							  absolute value of the mtype argument.
	 */
	void receive(void* buffer, size_t size, long mtype);

	/**
	 *  Immediately removes the message queue, awakening all waiting reader and
	 *  writer processes (with an error return and errno set to EIDRM). The
	 *  calling process must be the creator the message queue. Only one instance
	 *  of RawMessageQueue (created by the same arguments) has the member
	 *  _freeOnExit seted in true. If _freeOnExit's value is true, the message
	 *  queue is removed.
	 */
	virtual ~RawMessageQueue();

protected:
	int _queueId;
	bool  _freeOnExit;

	// Non copiable.
	DECLARE_NON_COPIABLE(RawMessageQueue)
};

/**
 * A class responsible for inter-process communication.
 */
class MessageQueue : public RawMessageQueue
{
public:

	/**
	 * Connect to a message queue, or create it if it doesn't exist
	 *
	 * @param pathName     A path to an existing file.
	 * @param id       A char to identify the shared memory object.
	 */
	explicit MessageQueue(const string& pathName, char id):
		RawMessageQueue(pathName,id)
	{ }

	/**
	 * Send an object through the queue. Note that the object must be of scalar
	 * type (or it's internal structure be all scalar types), as it will be
	 * written as bytes.
	 *
	 * @param obj	The object to be send to the queue.
	 * @param mtype	The id of the message.
	 */
	template <typename T>
	void send(const T& obj, long mtype)
	{
		RawMessageQueue::send(&obj,sizeof(T),mtype);
	}

	/**
	 * Receives an object from the queue. Note that the object must be of
	 * scalar type (or it's internal structure be all scalar types), as it
	 * will be written as bytes.
	 *
	 * @param mtype	The id of the message.
	 * @return 		The 'T' object read from the queue.
	 */
	template <typename T>
	T receive(long mtype)
	{
		T obj;
		RawMessageQueue::receive(&obj,sizeof(T),mtype);
		return obj;
	}

	/**
	 * Send a string message through the queue.
	 *
	 * @param message	The message to be send to the queue.
	 * @param mtype		The id of the message.
	 */
	void send(const std::string& message, long mtype);

	/**
	 * Receives a string message from the queue.
	 *
	 * @param mtype	The id of the message.
	 * @return 		The string message read from the queue.
	 */
	const std::string receive(long mtype);

	/**
	 * Removes the message queue. The calling process must be the creator the
	 * message queue
	 */
	virtual ~MessageQueue()
	{ }
};

#endif /* MESSAGE_QUEUE_H_ */
