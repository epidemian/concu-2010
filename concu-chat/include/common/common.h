/*
 * arg_parse.h
 *
 *  Created on: Jul 12, 2010
 *      Author: demian
 */

#ifndef ARG_PARSE_H_
#define ARG_PARSE_H_

/**
 * Parses the command-line arguments for both client and server.
 */
void parseArguments(int argc, char* argv[]);

/**
 * Loads the configuration file for both client and server and sets it as the
 * gloabl config.
 */
void loadConfigFile();

#endif /* ARG_PARSE_H_ */
