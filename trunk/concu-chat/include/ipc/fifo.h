/*
 * fifo.h
 *
 *  Created on: 10/04/2010
 *      Author: nicolas
 */

#ifndef IPC_FIFO_H_
#define IPC_FIFO_H_

#include "utils.h"
#include "exception.h"

#include <string>

using std::string;

/** The runtime error type thrown by Fifo. */
class FifoError: public Exception
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
	explicit Fifo(const string& pathName);

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

	DECLARE_NON_COPIABLE(Fifo);
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
	explicit FifoWriter(const Fifo& fifo);

	/**
	 * Closes the file of the fifo.
	 */
	virtual ~FifoWriter();

	/**
	 * Writes to the fifo.
	 *
	 * @param buffer Data to be written on the fifo.
	 * @param size   The size, in bytes, of the data.
	 * @return       The number of bytes written to the fifo. Might be less than
	 *               size.
	 */
	size_t write(const void* buffer, size_t size);

	/**
	 * Writes a given number of bytes to the fifo.
	 *
	 * @param buffer Data to be written on the fifo.
	 * @param size   The size, in bytes, of the data.
	 */
	void writeFixedSize(const void* buffer, size_t size);

	/**
	 * Writes an object to the pipe. Note that the object must be of scalar type
	 * (or it's internal structure be all scalar types), as it will be written
	 * as bytes.
	 *
	 * @param obj The object to be written to the pipe.
	 */
	template <typename T> void write(const T& obj)
	{
		writeFixedSize(&obj, sizeof(T));
	}

private:
	int _fileDescriptor;

	DECLARE_NON_COPIABLE(FifoWriter);
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
	explicit FifoReader(const Fifo& fifo);

	/**
	 * Closes the file of the fifo.
	 */
	virtual ~FifoReader();

	/**
	 * Reads from the fifo.
	 *
	 * @param buffer Destination of the data.
	 * @param size   The size, in bytes, of the data to be read.
	 * @return       The number of bytes read and stored into the buffer. Might
	 *               be less than size. Zero indicates end of file.
	 */
	size_t read(void* buffer, size_t size);

	/**
	 * Reads a given number of bytes from the fifo. In case less bytes could be
	 * read, an exception is thrown.
	 *
	 * @param buffer Destination of the data.
	 * @param size   The size, in bytes, of the data to be read.
	 */
	void readFixedSize(void* buffer, size_t size);

	/**
	 * Reads an object from the pipe. Note that the object must be of scalar
	 * type (or it's internal structure be all scalar types), as it will be
	 * written as bytes.
	 *
	 * @param obj The object to be read from the pipe.
	 */
	template <typename T> void read(T& obj)
	{
		readFixedSize(&obj, sizeof(T));
	}

private:
	int _fileDescriptor;

	DECLARE_NON_COPIABLE(FifoReader);
};

#endif /* FIFO_H_ */
