/*
 * fifo.h
 *
 *  Created on: 10/04/2010
 *      Author: nicolas
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <string>
#include <stdexcept>

using std::string;

/** The runtime error type thrown by Fifo. */
class FifoError: public std::runtime_error
{ public: FifoError(const string& msj, int errorCode = 0); };

/**
 * A class responsible for inter-process communication. Fifo (named pipe) is
 * system-persistent and exists beyond the life of the process and must be
 * deleted once it is no longer being used.
 */

class Fifo
{
public:
	/**
	 * Creates a named-pipe.
	 *
	 * @param pathName     A path to a non-existing file.
	 */
	Fifo(const string& pathName);

	/**
	 * Unlinks the associated file to the fifo from the file-system.
	 */
	virtual ~Fifo();

	/**
	 * Returns the pathName of the fifo.
	 */
	const string getPathName() const { return this->_pathName; }

private:
	const string _pathName;

};

/**
 * A class responsible for writing on the fifo shared between two processes.
 */
class FifoWriter
{
public:
	/*
	 * Opens a named-pipe in writing mode.
	 *
	 * @param Fifo     A named-pipe that is use to get the pathName to open it.
	 */
	FifoWriter(const Fifo& fifo);

	/**
	 * Closes the file of the fifo.
	 */
	virtual ~FifoWriter();

	/**
	 * Writes on the fifo.
	 *
	 * @param buffer     Data to be written on the fifo.
	 * @param size       The size, in bytes, of the data.
	 */

	int write(const char* buffer, int size);

private:
	int _fileDescriptor;
};

/**
 * A class responsible for reading on the fifo shared between two processes.
 */
class FifoReader
{
public:
	/*
	 * Opens a named-pipe in reading mode.
	 *
	 * @param Fifo     A named-pipe that is use to get the pathName to open it.
	 */
	FifoReader(const Fifo& fifo);

	/**
	 * Closes the file of the fifo.
	 */
	virtual ~FifoReader();

	/**
	 * Reads from the fifo.
	 *
	 * @param buffer     Destination of the data.
	 * @param size       The size, in bytes, of the data to be read.
	 */

	int read(char* buffer, int size);

private:
	int _fileDescriptor;
};

#endif /* FIFO_H_ */
