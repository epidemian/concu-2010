#include "ipc/fifo.h"

#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <iostream>

// Helper functions.
namespace
{

bool isFifoFile(const string& pathName)
{
	struct stat fileStat;
	if (stat(pathName.c_str(), &fileStat) == -1)
		throw FifoError("isFifoFile(): Could not get file info", errno);

	return fileStat.st_mode & S_IFIFO;
}

} // end namespace.


FifoError::FifoError(const string& msj, int errorCode) :
	runtime_error(msj + " - Error: " + (errorCode ? strerror(errorCode) : ""))
{ }


Fifo::Fifo(const string& pathName) : _pathName(pathName)
{
	bool mknodEror = mknod(pathName.c_str(), S_IFIFO | 0666, 0) == -1;

	if (mknodEror)
	{
		bool fileAlreadyExists = errno == EEXIST;
		if (fileAlreadyExists)
		{
			if (!isFifoFile(pathName))
				throw FifoError("Fifo(): File " + pathName + " already exists, "
			                "but it's not a fifo");
		}
		else
			throw FifoError("Fifo(): Could not create fifo file", errno);
	}

}


Fifo::~Fifo()
{
	bool unlinkError = unlink(_pathName.c_str()) == -1;

	if (unlinkError && errno != ENOENT) // ENOENT = No such file.
		perror("~Fifo(): Could not unlink the fifo's file");
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
			throw FifoError("FifoWriter::writeFixedSize(): Could not write to "
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
			throw FifoError("FifoReader::readFixedSize(): Unexpected EOF");

		totalBytes += bytes;
		index += bytes;
	 }
}
