cmake_minimum_required(VERSION 2.8.0)
project(ThreadPool)

set(EXECUTABLE_OUTPUT_PATH "${ThreadPool_SOURCE_DIR}/bin" CACHE PATH "Path to place built executables.")
set(LIBRARY_OUTPUT_PATH "${ThreadPool_SOURCE_DIR}/bin" CACHE PATH "Path to place built library objects (static and shared).")

set(BOOST_VERSION "1.48.0" CACHE STRING "Boost version to use.")
set(TestSuite_PATH "${ThreadPool_SOURCE_DIR}/../TestSuite" CACHE PATH "Path to TestSuite config files.")

set(sDir "${ThreadPool_SOURCE_DIR}")
set(bDir "${ThreadPool_BINARY_DIR}")
set(eDir "${EXECUTABLE_OUTPUT_PATH}")
set(lDir "${LIBRARY_OUTPUT_PATH}")

string(TOUPPER "${CMAKE_BUILD_TYPE}" bType)

if(sDir STREQUAL bDir)
	message(FATAL_ERROR "In source builds are NOT supported.")
endif(sDir STREQUAL bDir)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-std=c++0x")
	set(CMAKE_CXX_FLAGS_DEBUG "-std=c++0x")
	set(CMAKE_CXX_FLAGS_RELEASE "-std=c++0x")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)
	add_definitions(/EHa)
endif(WIN32)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost ${BOOST_VERSION} COMPONENTS date_time thread)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(TestSuite_INCLUDE_DIRS ${TestSuite_PATH}/include)
set(TestSuite_BOILERPLATE_SOURCE ${TestSuite_PATH}/include/UnitTestBP)

# Setup
include_directories("${sDir}/include")
link_directories("${lDir}")

add_subdirectory(src)

add_subdirectory(tests)
