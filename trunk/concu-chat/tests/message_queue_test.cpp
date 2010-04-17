/*
 * message_queue_test.cpp
 *
 *  Created on: 16/04/2010
 *      Author: nicolas
 */

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>

#include "ipc/message_queue.h"
#include "message.h"

using std::string;

const char ID = 'i';

int test1(int argc, char **argv);
int test2(int argc, char **argv);

int father(pid_t firSonPid, pid_t secSonPid, const string& pathName);
int son1(const string& pathName);
int son2(pid_t brothersPid, const string& pathName);

int main(int argc, char **argv)
{
	std::cout << "Message Queue Testing";


	std::cout << "\n\nMessage Queue Test 1 begins\n\n";
	test1(argc,argv);

	std::cout << "\n\nMessage Queue Test 2 begins\n\n";
	test2(argc,argv);
}

int test1(int argc, char **argv)
{
	string pathName = argv[0];
	MessageQueue queue(pathName,ID);

	Message m1("This is our first message!");
	std::cout << "We send a message: " << m1.getMessage() << "\n";
	queue.write(m1,getpid());

	sleep(2);

	Message m2;
	queue.read(m2,getpid());
	std::cout << "We receive a message: '" << m2.getMessage() << "'\n";

	return 0;
}

int test2(int argc, char **argv)
{
	string pathName = argv[0];

	pid_t pid = fork();

	if (pid == 0)
	{
		sleep(2);
		/**
		 * First son's code.
		 */
		return son1(pathName);
	}
	else
	{
		/**
		 * Father's code.
		 */
		pid_t brothersPid = pid;
		pid = fork();

		if (pid == 0)
		{
			sleep(2);
			/**
			 * Second son's code.
			 */
			return son2(brothersPid,pathName);
		}
		else
		{
			/**
			 * Father's code.
			 */
			return father(brothersPid,pid,pathName);
		}
	}
}

int father(pid_t firSonPid, pid_t secSonPid, const string& pathName)
{
	/*
	 * std::cout << "I'm the father. My process id is: " << getpid() << "\n";
	 * std::cout << "My first sun process id is: " << firSonPid << "\n";
	 * std::cout << "My second sun process id is: " << secSonPid << "\n";
	 */

	MessageQueue queue(pathName,ID);

	/**
	 * Father sends a message saying hi to his first son.
	 */
	std::cout << "Father sends a message to his first son\n";
	Message m_son1("Hi first son!");
	queue.write(m_son1,firSonPid);

	sleep(0);

	/**
	 * Father sends a message saying hi to his second son.
	 */
	std::cout << "Father sends a message to his second son\n";
	Message m_son2("Hi second son!");
	queue.write(m_son2,secSonPid);

	/**
	 * We wait the sons.
	 */
	waitpid(firSonPid, NULL, 0);
	waitpid(secSonPid, NULL, 0);

	return 0;
}

int son1(const string& pathName)
{
	/*
	 * std::cout << "I'm the first son. My process id is: " << getpid() << "\n";
	 * std::cout << "My father process id is: " << getppid() << "\n";
	 */

	MessageQueue queue(pathName,ID);

	/**
	 * Receives a message from his father.
	 */
	Message m_father;
	queue.read(m_father,getpid());

	std::cout << "Son1 received a message: " << m_father.getMessage() << "\n";

	/**
	 * Receives a message from his brother.
	 */
	Message m_brother;
	queue.read(m_brother,getpid());

	std::cout << "Son1 received a message: " << m_brother.getMessage() << "\n";

	return 0;
}

int son2(pid_t brothersPid, const string& pathName)
{
	/*
	 * std::cout << "I'm the second son. My process id is: " << getpid() << "\n";
	 * std::cout << "My father process id is: " << getppid() << "\n";
	 */

	MessageQueue queue(pathName,ID);

	/**
	 * Receives a message from his father.
	 */
	Message m_father;
	queue.read(m_father,getpid());

	std::cout << "Son2 receives a message: " << m_father.getMessage() << "\n";

	//std::cout << "Son2 receives a message\n";

	/**
	 * Father sends a message saying hi to his second son.
	 */
	Message m_brother("Hi brothe!\0");
	queue.write(m_brother,long(brothersPid));

	std::cout << "Son2 sends a message to his brother\n";

	return 0;
}
