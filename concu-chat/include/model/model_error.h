/*
 * model_error.h
 *
 *  Created on: May 11, 2010
 *      Author: demian
 */

#ifndef MODEL_ERROR_H_
#define MODEL_ERROR_H_

#include "exception.h"

class ModelError: public Exception
{
public:
	ModelError(const string& msj): Exception(msj)
	{ }
};


#endif /* MODEL_ERROR_H_ */
