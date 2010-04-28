/*
 * resource.cpp
 *
 *  Created on: Apr 23, 2010
 *      Author: demian
 */

#include "ipc/resource.h"
#include "ipc/resource_collector.h"

#include <iostream>

using std::cout;
using std::cerr;

Resource::Resource(bool ownResources) :
	_ownResources(ownResources),
	_disposed(false)
{
	ResourceCollector::instance().registerResource(this);
}


Resource::~Resource() throw ()
{
	ResourceCollector::instance().unregisterResource(this);
}

bool Resource::ownResources() const        { return _ownResources; }
void Resource::setOwnResources(bool value) { _ownResources = value; }

void Resource::dispose() throw ()
{
	if (!_disposed)
	{
		doDispose();
		_disposed = true;
	}
	else
		cerr << "Resource::dispose(): resource already disposed";
}
