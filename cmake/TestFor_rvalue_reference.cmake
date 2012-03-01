
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

if("TestFor_rvalue_reference" MATCHES "^")
	message(STATUS "Check for rvalue reference")
	try_compile(THREADPOOL_HAVE_RVALUE_REFERENCE ${CMAKE_BINARY_DIR} ${ThreadPool_SOURCE_DIR}/cmake/TestFor_rvalue_reference.cpp OUTPUT_VARIABLE OUTPUT)
	if(THREADPOOL_HAVE_RVALUE_REFERENCE)
		message(STATUS "Check for rvalue reference - found")
		file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILE_DIRECTORY}/CMakeOutput.log "Determining if the compiler has rvalue reference support passed with the following output:\n${OUTPUT}\n\n")
	else(THREADPOOL_HAVE_RVALUE_REFERENCE)
		message(STATUS "Check for rvalue reference - not found")
		file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILE_DIRECTORY}/CMakeOutput.log "Determining if the compiler has rvalue reference support failed with the following output:\n${OUTPUT}\n\n")
	endif(THREADPOOL_HAVE_RVALUE_REFERENCE)
endif("TestFor_rvalue_reference" MATCHES "^")
