/*
 * resource_collector.cpp
 *
 *  Created on: Apr 23, 2010
 *      Author: demian
 */

#include "ipc/resource_collector.h"
#include "ipc/resource.h"

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <cstdlib>



ResourceCollector::ResourceCollector()
{
}

ResourceCollector::~ResourceCollector()
{
	disposeAllResources();
}

ResourceCollector& ResourceCollector::instance()
{
	static ResourceCollector singleton;
	return singleton;
}

void ResourceCollector::registerResource(Resource* res)
{
	if (!alreadyRegistered(res))
		_resources.push_back(res);
	else
		throw std::invalid_argument("ResourceCollector::registerResource(): "
				"resource already registered");
}

void ResourceCollector::unregisterResource(Resource* res)
{
	if (alreadyRegistered(res))
		_resources.remove(res);
	else
		throw std::invalid_argument("ResourceCollector::unregisterResource(): "
				"resource is not registered");
}

bool ResourceCollector::alreadyRegistered(Resource* res) const
{
	ResourceList::const_iterator it =
			std::find(_resources.begin(),_resources.end(), res);
	return it != _resources.end();
}

void ResourceCollector::disposeAllResources()
{
	while (!_resources.empty())
	{
		Resource* res = _resources.front();
		res->dispose();
		_resources.pop_front();
	}
}
