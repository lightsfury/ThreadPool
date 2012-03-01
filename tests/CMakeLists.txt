if(EXECUTABLE_OUTPUT_PATH)
	set(EXECUTABLE_OUTPUT_PATH "${eDir}/tests")
endif()

if(LIBRARY_OUTPUT_PATH)
	set(LIBRARY_OUTPUT_PATH "${lDir}/tests")
endif()

include_directories("../src" "${TestSuite_INCLUDE_DIRS}")
link_directories("${lDir}" "${lDir}/tests")

file(GLOB TestModules "*.cpp")
foreach(TestFile ${TestModules})
	message(${TestFile})
	string(REGEX REPLACE "^.*[/\\](.*).cpp$" "\\1" TestName ${TestFile})
	add_library(${TestName} MODULE ${TestName} "${TestSuite_BOILERPLATE_SOURCE}")
endforeach(TestFile)
