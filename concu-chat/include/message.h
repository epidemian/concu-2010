/*
 * message.h
 *
 *  Created on: 16/04/2010
 *      Author: nicolas
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <cstring>

class Message
{
public:
	explicit Message(const char* message = "")
	{
		setMessage(message);
	}

	const char* getMessage() { return _message; }

private:

	static const size_t NAME_SIZE = 256;

	char _message[NAME_SIZE];

	void setMessage(const char* message)
	{
		strncpy(_message, message, NAME_SIZE);
	}
};

#endif /* MESSAGE_H_ */
