/*
 * resource.h
 *
 *  Created on: Apr 19, 2010
 *      Author: demian
 */

#ifndef IPC_RESOURCE_H_
#define IPC_RESOURCE_H_

#include "utils.h"

#include <iostream>

using std::ostream;

class Resource
{
public:
	virtual ~Resource() throw ();

	virtual bool ownResources() const;
	virtual void setOwnResources(bool value);

	virtual void print(ostream& stream) const = 0;

protected:
	Resource(bool ownResources);

	virtual void doDispose() throw () = 0;

private:
	bool _ownResources;
	bool _disposed;

	// ResourceCollector may call dispose(), but no one else! Not even Resource
	// subclasses!
	friend class ResourceCollector;

	void dispose() throw ();

	DECLARE_NON_COPIABLE(Resource)
};


inline ostream& operator << (ostream& stream, const Resource& res)
{
	res.print(stream);
	return stream;
}

#endif /* RESOURCE_H_ */
