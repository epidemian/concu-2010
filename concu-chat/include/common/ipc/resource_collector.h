/*
 * resource_collector.h
 *
 *  Created on: Apr 23, 2010
 *      Author: demian
 */

#ifndef IPC_RESOURCE_COLLECTOR_H_
#define IPC_RESOURCE_COLLECTOR_H_

#include "utils.h"
#include <list>

using std::list;

class Resource;

class ResourceCollector
{
public:
	/** Singleton instance. */
	static ResourceCollector& instance();

	void registerResource(Resource* res);
	void unregisterResource(Resource* res);

private:
	typedef list<Resource*> ResourceList;
	ResourceList _resources;

	/** Private constructor and destructor. */
	ResourceCollector();
	~ResourceCollector();

	bool alreadyRegistered(Resource* res) const;
	void disposeAllResources();

	DECLARE_NON_COPIABLE(ResourceCollector)
};


#endif /* RESOURCE_COLLECTOR_H_ */
