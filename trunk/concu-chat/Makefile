################################################################################
#                                                                              #
# LD-Soft Makefile Template                                                    #
#                                                                              #
# Author: Demian Ferreiro                                                      #
# Created: 2009/04/18                                                          #
#                                                                              #
# Copyleft 2009 LD-Soft - Under the GPL license (http://www.gnu.org)           #
#                                                                              #
################################################################################


##### BASIC CONFIGURATION ######################################################

# Programs' names.
client = client
server = server
shmtest = shared-memory-test
fifotest = fifo-test
exceptiontest = exceptions-test
messagetest = message-queue-test
semaphoretest = semaphore-test
producertest = producer-consumer-test

# Programs to build.
programs = $(server)\
 $(client)\
 $(shmtest)\
 $(fifotest)\
 $(exceptiontest)\
 $(messagetest)\
 $(semaphoretest)\
 $(producertest)

# Language: C or C++
language = C++

# Common source file directories. Make will search recursively into theese for
# source files to build.  
common_src_dirs = src/common

# Common header directories. Will be included with -I option.
include_dirs = include/common include/server include/client

# Program-specific source directories. 
# Variable names must be <program name>_src_dirs
$(client)_src_dirs = src/client
$(server)_src_dirs = src/server

$(shmtest)_src_files       = tests/shared_memory_test.cpp
$(fifotest)_src_files      = tests/fifo_test.cpp
$(exceptiontest)_src_files = tests/exceptions_test.cpp
$(messagetest)_src_files   = tests/message_queue_test.cpp
$(semaphoretest)_src_files = tests/semaphore_test.cpp
$(producertest)_src_files  = tests/producer_consumer_test.cpp

# Program-scpecific LDFLAGS. (CFLAGS are global... sorry)
$(client)_ldflags =
$(server)_ldflags =

# Uncomment to make a static compilation (all linked libraries are included into
# the program executable). 
#static = yes

# Uncomment to make the compilation process more verbose.
verbose = yes


##### ADVANCED CONFIGURATION ###################################################

# Linker options.
LDFLAGS = -rdynamic

# Compiler options.
#CFLAGS =


##### RULES ####################################################################

# Uses the common rules.
include Makefile.common

