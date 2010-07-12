/*
 * message.h
 *
 *  Created on: 16/04/2010
 *      Author: nicolas
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "core/byte_array.h"
#include "core/serializable.h"
#include <string>

using std::string;

class Message: public Serializable
{
public:

	/**
	 * The different types of messages.
	 */
	enum MessageType
	{
		/** Invalid message type */
		TYPE_NONE = 0,

		/**
		 * Sent from input process to main client process when user enters
		 * some text on the screen.
		 */
		TYPE_USER_INPUT,

		/**
		 * Sent from input process to main client process when user enters the
		 * exit command.
		 */
		TYPE_USER_EXIT,

		/** Sent to server to register a new name. */
		TYPE_REGISTER_NAME_REQUEST,

		/**
		 * Sent from server to client to inform whether the name was registered
		 * or not.
		 */
		TYPE_REGISTER_NAME_RESPONSE,

		/** Sent to server to unregister a name. */
		TYPE_UNREGISTER_NAME_REQUEST,

		/** Sent to server to make it show the peer table. */
		TYPE_SHOW_PEER_TABLE_REQUEST,

		/** Sent from client to server to request the current peer table. */
		TYPE_PEER_TABLE_REQUEST,

		/**
		 * Sent from server to client in response of the peer table request.
		 * Contains the peer table data.
		 */
		TYPE_PEER_TABLE_RESPONSE,

		/**
		 * Sent from peer to peer client to let the second know that the first
		 * wants to start a chat session.
		 */
		TYPE_START_CHAT_REQUEST,

		/**
		 * Sent from peer to peer in response of a start chat request by the
		 * second  informing whether the first wants to chat or not.
		 */
		TYPE_START_CHAT_RESPONSE,

		/**
		 * Sent from peer to peer to inform the second that the first has left
		 * the chat session.
		 */
		TYPE_END_CHAT,

		/**
		 * Sent from peer to peer. A chat message.
		 */
		TYPE_CHAT_MESSAGE,

		/**
		 * Sent to server to make it quit remotely.
		 */
		TYPE_SERVER_EXIT
	};

	Message() :
		_type(TYPE_NONE), _messengerPid(0)
	{
	}

	Message(MessageType type, pid_t messengerPid, const ByteArray& data =
			ByteArray()) :
		_type(type), _messengerPid(messengerPid), _data(data)
	{
	}

	MessageType getType() const
	{
		return _type;
	}

	pid_t getMessengerPid() const
	{
		return _messengerPid;
	}

	const ByteArray& getData() const
	{
		return _data;
	}

	virtual ByteArray serialize()
	{
		ByteArrayWriter writer;
		writer.write(_type);
		writer.write(_messengerPid);
		writer.writeByteArray(_data);
		return writer.getByteArray();
	}

	virtual void deserialize(const ByteArray& bytes)
	{
		ByteArrayReader reader(bytes);
		_type = reader.read<MessageType> ();
		_messengerPid = reader.read<pid_t> ();
		_data = reader.readAll();
	}

private:
	MessageType _type;
	pid_t _messengerPid;
	ByteArray _data;
};

#endif /* MESSAGE_H_ */
