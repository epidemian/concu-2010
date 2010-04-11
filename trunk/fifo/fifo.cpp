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
	if ((_fileDescriptor = open(fifo.getPathName().c_str(), O_WRONLY)) == -1)
		throw FifoError("FifoWriter(): Could not open" + fifo.getPathName(), errno);
}

FifoWriter::~FifoWriter()
{
	if (close(_fileDescriptor) == -1)
		perror("~FifoWriter(): Could not close the fifo's file");
}

int FifoWriter::write(const char* buffer, int size)
{
	pwrite((char*)&size,sizeof(int));
	return pwrite(buffer,size);
}

int FifoWriter::pwrite(const char* buffer, int size)
{
	int bytesToSend = size, bytesSended = 0;
	char* index = (char*)buffer;

	while (bytesToSend)
	{
		bytesSended = ::write(_fileDescriptor,buffer,size);

		if (bytesSended <= 0)
		{
			perror("~FifoWriter(): Could not write the fifo");
			return bytesSended;
		}

		bytesToSend -= bytesSended;
		index += bytesSended;
	 }

	return bytesSended;
}

FifoReader::FifoReader(const Fifo& fifo)
{
	if ((_fileDescriptor = open(fifo.getPathName().c_str(), O_RDONLY)) == -1)
		throw FifoError("FifoReader(): Could not open" + fifo.getPathName(), errno);
}

FifoReader::~FifoReader()
{
	if (close(_fileDescriptor) == -1)
		perror("~FifoReader(): Could not close the fifo's file");
}

int FifoReader::read(char* buffer)
{
	int bytesToReceived = 0;
	pread((char*)&bytesToReceived, sizeof(int));

	return pread(buffer, bytesToReceived);
}

int FifoReader::pread(char* buffer, int size){

	int bytesToReceived = size, bytesReceived = 0;
	char* index = (char*)buffer;

	while (bytesToReceived)
	{
		bytesReceived = ::read(_fileDescriptor,buffer,size);

		if (bytesReceived <= 0)
		{
			perror("~FifoReader(): Could not read from fifo");
			return bytesReceived;
		}

		bytesToReceived -= bytesReceived;
		index += bytesReceived;
	 }

	return bytesReceived;
}
