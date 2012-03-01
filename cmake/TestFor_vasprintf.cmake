
#=============================================================================
# Copyright 2011 Robert Beam
# Based on TestForANSIForScope.cmake
# 
	#=============================================================================
	# Copyright 2002-2009 Kitware, Inc.
	#
	# Distributed under the OSI-approved BSD License (the "License");
	# see accompanying file Copyright.txt for details.
	#
	# This software is distributed WITHOUT ANY WARRANTY; without even the
	# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	# See the License for more information.
	#=============================================================================
	# (To distributed this file outside of CMake, substitute the full
	#  License text for the above reference.)
#
#=============================================================================

if("TestFor_vasprintf" MATCHES "^TestFor_vasprintf$")
	message(STATUS "Check for GNU lib C vasprintf function")
	try_compile(THREADPOOL_HAVE_VASPRINTF ${CMAKE_BINARY_DIR} ${ThreadPool_SOURCE_DIR}/cmake/TestFor_vasprintf.c OUTPUT_VARIABLE OUTPUT)
	if(THREADPOOL_HAVE_VASPRINTF)
		message(STATUS "Check for GNU lib C vasprintf function - found")
		file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILE_DIRECTORY}/CMakeOutput.log "Determining if the C library has GNU lib C vasprintf function passed with the following output:\n${OUTPUT}\n\n")
	else(THREADPOOL_HAVE_VASPRINTF)
		message(STATUS "Check for GNU lib C vasprintf function - not found")
		file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILE_DIRECTORY}/CMakeError.log "Determining if the C library has GNU lib C vasprintf function failed with the following output:\n${OUTPUT}\n\n")
	endif(THREADPOOL_HAVE_VASPRINTF)
endif("TestFor_vasprintf" MATCHES "^TestFor_vasprintf$")
