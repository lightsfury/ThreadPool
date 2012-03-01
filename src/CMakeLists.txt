add_library(ThreadPool MODULE ThreadPool)
add_library(ThreadPool_static STATIC ThreadPool)

set_property(TARGET ThreadPool PROPERTY COMPILE_DEFINITIONS THREADPOOL_EXPORTS)

if(NOT MSVC)
	target_link_libraries(ThreadPool boost_thread boost_date_time)
	target_link_libraries(ThreadPool_static boost_thread boost_date_time)
endif(NOT MSVC)
