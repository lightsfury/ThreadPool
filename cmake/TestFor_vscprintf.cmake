
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

if("TestFor_vscprintf" MATCHES "^TestFor_vscprintf$")
	message(STATUS "Check for MSVC _vscprintf function")
	try_compile(THREADPOOL_HAVE_VSCPRINTF ${CMAKE_BINARY_DIR} ${ThreadPool_SOURCE_DIR}/cmake/TestFor_vscprintf.c OUTPUT_VARIABLE OUTPUT)
	if(THREADPOOL_HAVE_VSCPRINTF)
		message(STATUS "Check for MSVC _vscprintf function - found")
		file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILE_DIRECTORY}/CMakeOutput.log "Determining if the C library has MSVC _vscprintf function passed with the following output:\n${OUTPUT}\n\n")
	else(THREADPOOL_HAVE_VSCPRINTF)
		message(STATUS "Check for MSVC _vscprintf function - not found")
		file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILE_DIRECTORY}/CMakeError.log "Determining if the C library has MSVC _vscprintf function failed with the following output:\n${OUTPUT}\n\n")
	endif(THREADPOOL_HAVE_VSCPRINTF)
endif("TestFor_vscprintf" MATCHES "^TestFor_vscprintf$")
