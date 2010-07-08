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

using std::string;

const char ID = 'i';

int test1(int argc, char **argv);
int test2(int argc, char **argv);

int parent(pid_t firSonPid, pid_t secSonPid, const string& pathName);
int child1(const string& pathName);
int child2(pid_t brothersPid, const string& pathName);

int main(int argc, char **argv)
{
	std::cout << "Message Queue Testing";

	//	std::cout << "\n\nMessage Queue Test 1 begins\n\n";
	//	test1(argc,argv);

	std::cout << "\n\nMessage Queue Test 2 begins\n\n";
	test2(argc, argv);
}

int test1(int argc, char **argv)
{
	string pathName = argv[0];
	MessageQueue queue(pathName, ID);

	std::string m1("This is our first message!");
	std::cout << "We send a message: " << m1 << "\n";
	queue.sendString(m1, getpid());

	sleep(2);

	std::string m2 = queue.receiveString(getpid());
	std::cout << "We receive a message: '" << m2 << "'\n";

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
		return child1(pathName);
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
			return child2(brothersPid, pathName);
		}
		else
		{
			/**
			 * Father's code.
			 */
			return parent(brothersPid, pid, pathName);
		}
	}
}

int parent(pid_t firSonPid, pid_t secSonPid, const string& pathName)
{
	/*
	 * std::cout << "I'm the father. My process id is: " << getpid() << "\n";
	 * std::cout << "My first sun process id is: " << firSonPid << "\n";
	 * std::cout << "My second sun process id is: " << secSonPid << "\n";
	 */

	MessageQueue queue(pathName, ID);

	try
	{
		/**
		 * Father sends a message saying hi to his first son.
		 */
		std::string m_son1("Hi first son!");
		std::cout << "Father sends a message to his first son: " << m_son1
				<< "\n";
		queue.sendString(m_son1, firSonPid);

		/**
		 * Father sends a message saying hi to his second son.
		 */
		std::string m_son2("Hi second son!");
		std::cout << "Father sends a message to his second son: " << m_son2
				<< "\n";
		queue.sendString(m_son2, secSonPid);

		/**
		 * We wait the sons.
		 */
		waitpid(firSonPid, NULL, 0);
		waitpid(secSonPid, NULL, 0);
	} catch (...)
	{
	}

	return 0;
}

int child1(const string& pathName)
{
	/*
	 * std::cout << "I'm the first son. My process id is: " << getpid() << "\n";
	 * std::cout << "My father process id is: " << getppid() << "\n";
	 */

	MessageQueue queue(pathName, ID, false);

	try
	{
		/**
		 * Receives a message from his father.
		 */
		std::string m_father = queue.receiveString(getpid());
		std::cout << "Son1 received a message: " << m_father << "\n";

		/**
		 * Receives a message from his brother.
		 */
		std::string m_brother = queue.receiveString(getpid());
		std::cout << "Son1 received a message: " << m_brother << "\n";
	} catch (...)
	{
	}

	return 0;
}

int child2(pid_t brothersPid, const string& pathName)
{
	/*
	 * std::cout << "I'm the second son. My process id is: " << getpid() << "\n";
	 * std::cout << "My father process id is: " << getppid() << "\n";
	 */

	MessageQueue queue(pathName, ID, false);

	try
	{
		/**
		 * Receives a message from his father.
		 */
		std::string m_father = queue.receiveString(getpid());
		std::cout << "Son2 receives a message: " << m_father << "\n";

		/**
		 * Son2 sends a message to his brother.
		 */
		std::string m_brother("Hi brothe!");
		std::cout << "Son2 sends a message to his brother: " << m_brother
				<< "\n";
		queue.sendString(m_brother, long(brothersPid));
	} catch (...)
	{
	}

	return 0;
}

