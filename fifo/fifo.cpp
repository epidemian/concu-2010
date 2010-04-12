#include "fifo.h"

#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdio.h>

FifoError::FifoError(const string& msj, int errorCode) :
	runtime_error(msj + " - Error: " + (errorCode ? strerror(errorCode) : ""))
{ }

Fifo::Fifo(const string& pathName) : _pathName(pathName)
{
	mknod(pathName.c_str(), S_IFIFO|0666, 0);
	/* if (mknod(pathName.c_str(), S_IFIFO|0666, 0) == -1)
		throw FifoError("Fifo(): Could not create fifo", errno);
	*/
}

Fifo::~Fifo()
{
	/*if (unlink(_pathName.c_str()) == -1)
		perror("~Fifo(): Could not unlink the fifo's file");
	*/
	unlink(_pathName.c_str());
}

FifoWriter::FifoWriter(const Fifo& fifo)
{
	_fileDescriptor = open(fifo.getPathName().c_str(), O_WRONLY);
	if (_fileDescriptor == -1)
		throw FifoError("FifoWriter(): Could not open" + fifo.getPathName(),
		                errno);
}

FifoWriter::~FifoWriter()
{
	if (close(_fileDescriptor) == -1)
		perror("~FifoWriter(): Could not close the fifo's file");
}

size_t FifoWriter::write(const void* buffer, size_t size)
{
	int bytes = ::write(_fileDescriptor, buffer, size);
	if (bytes == -1)
		throw FifoError("FifoWriter::write(): Could not write to fifo", errno);

	return bytes;
}

void FifoWriter::writeFixedSize(const void* buffer, size_t size)
{
	size_t totalBytes = 0;
	char* index = (char*) buffer;

	while (totalBytes < size)
	{
		int bytes = write(index, size);

		if (bytes == 0)
			// Does not use errno.
			throw FifoError("FifoWriter::writeFixedSize(): could not write to "
			                "fifo");

		totalBytes += bytes;
		index += bytes;
	 }
}

FifoReader::FifoReader(const Fifo& fifo)
{
	_fileDescriptor = open(fifo.getPathName().c_str(), O_RDONLY);
	if (_fileDescriptor == -1)
		throw FifoError("FifoReader(): Could not open" + fifo.getPathName(),
		                errno);
}

FifoReader::~FifoReader()
{
	if (close(_fileDescriptor) == -1)
		perror("~FifoReader(): Could not close the fifo's file");
}

size_t FifoReader::read(void* buffer, size_t size)
{
	int bytes = ::read(_fileDescriptor, buffer, size);
	if (bytes == -1)
		throw FifoError("FifoReader::read(): Could not read from fifo", errno);

	return bytes;
}

void FifoReader::readFixedSize(void* buffer, size_t size)
{
	size_t totalBytes = 0;
	char* index = (char*) buffer;

	while (totalBytes < size)
	{
		int bytes = read(index, size);

		if (bytes == 0)
			// Does not use errno.
			throw FifoError("FifoReader::readFixedSize(): unexpected EOF");

		totalBytes += bytes;
		index += bytes;
	 }
}
