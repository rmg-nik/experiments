cmake_minimum_required (VERSION 3.17)

set (CMAKE_CXX_STANDARD 20)

project ("experiments")

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
endif()

file(GLOB V_GLOB LIST_DIRECTORIES true "*")
foreach(item ${V_GLOB})
	if(${item} MATCHES "^${CMAKE_CURRENT_SOURCE_DIR}/[0-9][0-9].*")
		add_subdirectory(${item})
	endif()
endforeach()
