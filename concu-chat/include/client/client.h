/*
 * client.h
 *
 *  Created on: 25/05/2010
 *      Author: nicolas
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "model/message.h"
#include <string>

using std::string;

class Client
{
public:

	Client(int argc, char* argv[]);

	~Client();

	int run();

private:
	string _queueFileName;

	void runUserInputProcess();
	void runMainProcess();

	void createQueueFile();
	void processMessage(const Message& message);
};

#endif /* CLIENT_H_ */
