/*
 * ipc_error.h
 *
 *  Created on: Apr 16, 2010
 *      Author: demian
 */

#ifndef IPC_ERROR_H_
#define IPC_ERROR_H_

#include "exception.h"

class IpcError: public Exception
{
public:
	IpcError(const string& msj, int errorCode = 0);

	int getErrorCode();

private:
	int _errorCode;
};


#endif /* IPC_ERROR_H_ */
