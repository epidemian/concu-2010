/*
 * global_config.h
 *
 *  Created on: Jul 12, 2010
 *      Author: demian
 */

#ifndef GLOBAL_CONFIG_H_
#define GLOBAL_CONFIG_H_

#include "config.h"
#include "utils.h"

#include <memory>

class GlobalConfig
{
public:

	template <typename T>
	static T get(const string& key)
	{
		if (!_config.get())
			throw Exception("No configuration has been set");
		return _config->get<T>(key);
	}

	typedef std::auto_ptr<Config> ConfigPtr;

	static void setConfig(ConfigPtr config);

private:
	static ConfigPtr _config;

	DECLARE_NON_COPIABLE(GlobalConfig)
};

#endif /* GLOBAL_CONFIG_H_ */
