cmake_minimum_required (VERSION 3.17)

set (CMAKE_CXX_STANDARD 20)

project ("experiments")

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
endif()

add_subdirectory ("00_multi_threaded_unique_id_exp")
add_subdirectory ("01_map_erase_exp")
add_subdirectory ("02_type_hash_exp")
add_subdirectory ("03_inheritance_exp")
add_subdirectory ("04_profiler_exp")
add_subdirectory ("05_allocation_exp")
add_subdirectory ("06_tmp_exp")
add_subdirectory ("07_static_dtro_exp")
add_subdirectory ("08_multithreaded_communication")
add_subdirectory ("09_event_types")
add_subdirectory ("10_enable_if_member_function")
