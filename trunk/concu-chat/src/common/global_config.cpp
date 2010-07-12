/*
 * global_config.cpp
 *
 *  Created on: Jul 12, 2010
 *      Author: demian
 */

#include "global_config.h"
#include <iostream>

GlobalConfig::ConfigPtr GlobalConfig::_config;

void GlobalConfig::setConfig(ConfigPtr config)
{
	std::cout << "setConfig! " << config.get() << "\n" << std::flush;

	_config = config;
}
