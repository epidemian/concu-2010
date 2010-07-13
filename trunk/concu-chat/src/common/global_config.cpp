/*
 * global_config.cpp
 *
 *  Created on: Jul 12, 2010
 *      Author: demian
 */

#include "global_config.h"
#include <iostream>

Config* GlobalConfig::_config = NULL;

void GlobalConfig::setConfig(Config* config)
{
	_config = config;
}
