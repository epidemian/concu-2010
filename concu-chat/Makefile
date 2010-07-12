################################################################################
#                                                                              #
# Multi-target Makefile                                                        #
#                                                                              #
# Author: Demian Ferreiro                                                      #
# Created: 2009/04/18                                                          #
#                                                                              #
################################################################################

  
##### BASIC CONFIGURATION ######################################################

# Programs' names.
client = client
server = server
serveradmin = server-admin
shmtest = shared-memory-test
fifotest = fifo-test
exceptiontest = exceptions-test
messagetest = message-queue-test
semaphoretest = semaphore-test
producertest = producer-consumer-test
bytearraytest = byte-array-test
filelocktest = file-lock-test

# Programs to build.
programs = $(server)\
 $(client)\
 $(serveradmin)\
 $(shmtest)\
 $(fifotest)\
 $(exceptiontest)\
 $(filelocktest)\
 $(messagetest)\
 $(semaphoretest)\
 $(producertest)\
 $(bytearraytest)\
 
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
$(serveradmin)_src_dirs = src/server_admin

$(shmtest)_src_files        = tests/shared_memory_test.cpp
$(fifotest)_src_files       = tests/fifo_test.cpp
$(exceptiontest)_src_files  = tests/exceptions_test.cpp
$(messagetest)_src_files    = tests/message_queue_test.cpp
$(semaphoretest)_src_files  = tests/semaphore_test.cpp
$(producertest)_src_files   = tests/producer_consumer_test.cpp
$(bytearraytest)_src_files  = tests/byte_array_test.cpp
$(filelocktest)_src_files   = tests/file_lock_test.cpp 

# Program-scpecific LDFLAGS. (CFLAGS are global... sorry)
$(client)_ldflags =
$(server)_ldflags =

# Uncomment to make a static compilation (all linked libraries are included into
# the program executable). 
#static = yes

# Uncomment to make the compilation process more verbose.
#verbose = yes


##### ADVANCED CONFIGURATION ###################################################

# Linker options.
LDFLAGS = -rdynamic

# Compiler options.
#CFLAGS =


##### RULES ####################################################################

# Uses the common rules.
include Makefile.common

