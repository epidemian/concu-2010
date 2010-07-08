/*
 * ipc_error.cpp
 *
 *  Created on: Apr 16, 2010
 *      Author: demian
 */

#include "ipc/ipc_error.h"

#include <cstring>

IpcError::IpcError(const string& msj, int errorCode) :
	Exception(msj + " - Error: " + (errorCode ? strerror(errorCode) : "")),
			_errorCode(errorCode)
{
}

int IpcError::getErrorCode()
{
	return _errorCode;
}
