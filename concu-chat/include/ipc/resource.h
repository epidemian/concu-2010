/*
 * resource.h
 *
 *  Created on: Apr 19, 2010
 *      Author: demian
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

class Resource
{
public:
	virtual ~Resource() throw () { }

	virtual bool ownResources() const { return _ownResources; }
	virtual void setOwnResources(bool value) { _ownResources = value; }

protected:
	Resource(bool ownResources):
		_ownResources(ownResources)
	{ }

private:
	bool _ownResources;
};

#endif /* RESOURCE_H_ */
