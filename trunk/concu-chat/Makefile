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
shmtest = shared-memory-test
fifotest = fifo-test
exceptiontest = exceptions-test

# Programs to build.
programs = $(shmtest) $(fifotest) $(exceptiontest)

# Language: C or C++
language = C++

# Common source file directories. Make will search recursively into theese for
# source files to build.  
common_src_dirs = src

# Common header directories. Will be included with -I option.
include_dirs = include

# Program-specific source directories. 
# Variable names must be <program name>_src_dirs
$(shmtest)_src_dirs       = 
$(fifotest)_src_dirs      = 
$(exceptiontest)_src_dirs = 

$(shmtest)_src_files       = tests/shared_memory_test.cpp
$(fifotest)_src_files      = tests/fifo_test.cpp
$(exceptiontest)_src_files = tests/exceptions_test.cpp

# Program-scpecific LDFLAGS. (CFLAGS are global... sorry)
$(hola)_ldflags =
$(adios)_ldflags =

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

